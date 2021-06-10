package keyboard

import spinal.core._
import spinal.lib._
import spinal.lib.bus.avalon._

case class PS2Rx() extends Component {
  val receiveFromHost: Stream[Bits] = slave Stream Bits(8 bits)
  val mm: AvalonMM = slave(AvalonMM(
    AvalonMMSlaveFactory.getAvalonConfig(2, 32)))
  mm addTag ClockDomainTag(ClockDomain.current)
  val bus: AvalonMMSlaveFactory = AvalonMMSlaveFactory(mm)
  bus.readStreamNonBlocking(receiveFromHost, 0, 8, 0)
  // TODO: FifoCC?
}
