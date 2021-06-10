package keyboard

import spinal.core._
import spinal.lib.com.uart._
import spinal.lib.fsm._
import spinal.lib._

case class HID() extends Component {
  val TXD: Bool = out Bool()
  val scanIdx: Flow[Bits] = slave Flow Bits(8 bits)
  val keyStatus: Vec[Flow[Bool]] = Vec(Flow(Bool), 103)
  val keyBits: Bits = in(Bits(keyStatus.asBits.getBitsWidth bits))
  keyStatus.assignFromBits(keyBits)

  val uartCtrl = new UartCtrl
  val uartConf: Area = new Area {
    uartCtrl.io.config.setClockDivider(9600 Hz)
    uartCtrl.io.config.frame.dataLength := 7  //8 bits
    uartCtrl.io.config.frame.parity := UartParityType.NONE
    uartCtrl.io.config.frame.stop := UartStopType.ONE
    uartCtrl.io.uart.rxd := True
    uartCtrl.io.writeBreak := False
    uartCtrl.io.write.payload := B(0)
    TXD := uartCtrl.io.uart.txd
  }
  val hidKeyTable = new Mem(Bits(8 bits), 128)
  hidKeyTable.addAttribute("ram_init_file", "../VerilogHDL/hid.mif")

  val idxLatch: UInt = RegNextWhen(U(scanIdx.payload(0, 7 bits)), scanIdx.valid, 0)
  val staLatch: Bool = RegNextWhen(scanIdx.payload(7), scanIdx.valid, False)

  val machine: StateMachine = new StateMachine {
    val idle = new State with EntryPoint
    val k6: State = new State {
      whenIsActive {
        when(uartCtrl.io.write.ready) (goto(idle))
      }
    }
    val k5: State = new State {
      whenIsActive {
        when(uartCtrl.io.write.ready) (goto(k6))
      }
    }
    val k4: State = new State {
      whenIsActive {
        when(uartCtrl.io.write.ready) (goto(k5))
      }
    }
    val k3: State = new State {
      whenIsActive {
        when(uartCtrl.io.write.ready) (goto(k4))
      }
    }
    val k2: State = new State {
      whenIsActive {
        when(uartCtrl.io.write.ready) (goto(k3))
      }
    }
    val k1: State = new State {
      whenIsActive {
        when(staLatch) {
          uartCtrl.io.write.payload := hidKeyTable.readSync(idxLatch)
        }
        when(uartCtrl.io.write.ready) (goto(k2))
      }
    }
    val reserved: State = new State {
      whenIsActive {
        when(uartCtrl.io.write.ready) (goto(k1))
      }
    }
    val special: State = new State {
      whenIsActive {
        val lCtrl = keyStatus(90).payload
        val lShift = keyStatus(74).payload
        val lAlt = keyStatus(92).payload
        val lWin = keyStatus(91).payload
        val rCtrl = keyStatus(96).payload
        val rShift = keyStatus(85).payload
        val rAlt = keyStatus(94).payload
        val rWin = False
        uartCtrl.io.write.payload := Cat(rWin, rAlt, rShift, rCtrl, lWin, lAlt, lShift, lCtrl)
        when(uartCtrl.io.write.ready) (goto(reserved))
      }
    }
    idle.whenIsActive {
      when(scanIdx.valid) (goto(special))
    }
    uartCtrl.io.write.valid := !isActive(idle)
  }
}
