package keyboard

import spinal.core._
import spinal.lib._
import spinal.lib.fsm._

case class PS2Device() extends Component{
  val PS2 = new ps2_bundle
  val receiveFromHost: Flow[Bits] = master Flow Bits(8 bits)
  val sendToHost: Stream[Bits] = slave Stream Bits(8 bits)

  val ps2ClkArea = new SlowArea(100 kHz) {
    val clockQuarter: Bool = Reg(Bool)
    clockQuarter := ~clockQuarter
  }
  val bus = new ps2_bus(
    ClockDomain.current.copy(ps2ClkArea.clockQuarter),
    80
  )
  bus.PS2 <> PS2
  bus.tx.valid := False
  bus.tx.payload := B(0)

  val tx_buf: Bits = RegNextWhen(sendToHost.payload, sendToHost.valid)
  sendToHost.ready := False
  receiveFromHost.payload := bus.rx.payload
  receiveFromHost.valid := False

  val machine: StateMachine = new StateMachine {
    val sending: Bool = bus.tx.ready.fall
    val idle: State = new State {
      whenIsActive {
        bus.tx.valid := False
        sendToHost.ready := True
        when (sendToHost.valid) { goto(tx) }
      }
    }
    val init: State = new State with EntryPoint {
      whenIsActive { //  TODO: no init
        bus.tx.valid := True
        bus.tx.payload := B(0xaa)
        when (~bus.tx_failed & sending) { goto(idle) }  // 1 clk
      }
    }
    val tx: State = new State {
      whenIsActive {
        bus.tx.valid := True
        bus.tx.payload := tx_buf
        when (sending) { goto(idle) }
      }
    }
    val rx: State = new State {
      whenIsActive {
        bus.tx.valid := False
      }
    }
    val ack: State = new State {
      whenIsActive {
        when (sending) {
          receiveFromHost.valid := True
          switch (bus.rx.payload) {
            is(0xff) { goto(init) }
            is(0xf3, 0xed) { goto(rx) }
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
