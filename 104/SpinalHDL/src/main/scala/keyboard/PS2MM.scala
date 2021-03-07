package keyboard

import spinal.core._
import spinal.lib._
import spinal.lib.bus.avalon._

case class PS2MM() extends Component {
  val sendToHost: Stream[Bits] = master Stream Bits(8 bits)
  val receiveFromHost: Stream[Bits] = slave Stream Bits(8 bits)
  val bus: AvalonMM = slave(AvalonMM(
    AvalonMMConfig.fixed(
      addressWidth = 2,
      dataWidth = 32,
      readLatency = 0
    ).copy(useWaitRequestn = false)
  ))

  val regs = new Bundle {
    val ctl: Bool = Bool
    val sta: Bits = Bits(5 bits)
    val txd, rxd = Bits(8 bits)
  }

  regs.txd <> sendToHost.payload
  regs.rxd <> receiveFromHost.payload
}
