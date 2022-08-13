package keyboard

import spinal.core._
import spinal.lib.com.uart._
import spinal.lib.fsm._

case class HID() extends Component {
  val TXD: Bool = out Bool()
  val scanIdx: Flow[Bits] = slave Flow Bits(8 bits)
  val keyStatus: Vec[Flow[Bool]] = Vec(Flow(Bool), 103)
  val keyBits: Bits = in(Bits(keyStatus.asBits.getBitsWidth bits))
  keyStatus.assignFromBits(keyBits)  // bootcamp

  val uartCtrl: UartCtrl = UartCtrl(
    config = UartCtrlInitConfig(
      baudrate = 9600,
      dataLength = 7,  // 8 bits
      parity = UartParityType.NONE,
      stop = UartStopType.ONE
    )
  )
  uartCtrl.io.uart.rxd := True  // High is the idle state for UART
  TXD := uartCtrl.io.uart.txd
  uartCtrl.io.write.payload := B(0)

  val hidKeyTable = new Mem(Bits(8 bits), 128)
  hidKeyTable.addAttribute("ram_init_file", "../VerilogHDL/hid.mif")

  val scanFifo: Stream[Bits] = scanIdx.toStream(
    overflow = False,
    fifoSize = 6,
    overflowOccupancyAt = 6
  )

  def getIdx(payload: Bits): UInt = U(payload(0, 7 bits))
  val getCond: Bool = scanFifo.payload(7) //&& RegNext(scanFifo.valid)
  val getCode: Bits = hidKeyTable.readSync(getIdx(scanFifo.payload))

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
        when (getCond) {
          uartCtrl.io.write.payload := getCode
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
      when(scanFifo.valid) (goto(special))
    }
    uartCtrl.io.write.valid := !isActive(idle)
    scanFifo.ready := isActive(k1)
  }
//  println(s"latency ${LatencyAnalysis(uartCtrl.io.write.payload, scanFifo.payload)}\n")
}
