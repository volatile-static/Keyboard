package keyboard

import spinal.core._
import spinal.lib._
import spinal.lib.fsm._

case class PS2Device() extends Component{
  val PS2_CLK, PS2_DAT = inout(Analog(Bool))  // tri
  val scanIdx: Flow[Bits] = slave Flow Bits(8 bits)

  val bus = new ps2_bus(80)
  bus.PS2_CLK <> PS2_CLK; bus.PS2_DAT <> PS2_DAT
  val ps2ClkArea: SlowArea = new SlowArea(100 kHz) {
    val clockQuarter: Bool = Reg(Bool)
    clockQuarter := ~clockQuarter
    bus.clock_quarter <> clockQuarter
  }

  bus.tx.valid := False
  bus.tx.payload := B(0)

  val machine: StateMachine = new StateMachine {
    val sending: Bool = bus.tx.ready.fall
    val idle: State = new State {
      whenIsActive {
        bus.tx.valid := False
      }
    }
    val init: State = new State with EntryPoint {
      whenIsActive {
        bus.tx.valid := True
        bus.tx.payload := B(0xaa)
        when (~bus.tx_failed & sending) { goto(idle) }  // 1 clk
      }
    }
//    val tx: State = new State {
//      whenIsActive {
//        bus.tx.valid := True
//        when (sending) { goto(idle) }
//      }
//    }
    val rx: State = new State {
      whenIsActive {
        bus.tx.valid := False
      }
    }
    val ack: State = new State {
      whenIsActive {
        when (sending) {
          switch (bus.rx.payload) {
            is(0xff) { goto(init) }
            is(0xed) { goto(rx) }
            is(0xf3) { goto(rx) }
            default { goto(idle) }
          }
        }
        bus.tx.valid := True
        bus.tx.payload := B(0xfa)
      }
    }
    always {
      when (bus.rx.valid) { goto(ack) }
    }
  }
}
