package keyboard

import spinal.core._
import spinal.lib._
import spinal.lib.bus.avalon._

case class PS2Rx() extends Component {
  val receiveFromHost: Flow[Bits] = slave Flow Bits(8 bits)
  val mm: AvalonMM = slave(AvalonMM(
    AvalonMMSlaveFactory.getAvalonConfig(2, 32)))
  mm addTag ClockDomainTag(ClockDomain.current)
  val bus: AvalonMMSlaveFactory = AvalonMMSlaveFactory(mm)

  val rxFifo: Stream[Bits] = receiveFromHost.toStream(
    overflow = False,
    fifoSize = 6,
    overflowOccupancyAt = 6
  )
  bus.readStreamNonBlocking(rxFifo, 0, 8, 0)
}
