package keyboard

import spinal.core._
import spinal.lib._
import spinal.lib.bus.avalon._

case class PS2Tx(fifoDepth : Int = 8) extends Component {
  val sendToHost: Stream[Bits] = master Stream Bits(8 bits)
  val mm: AvalonMM = slave(AvalonMM(
    AvalonMMSlaveFactory.getAvalonConfig(2, 32)))
  mm addTag ClockDomainTag(ClockDomain.current)
  val bus: AvalonMMSlaveFactory = AvalonMMSlaveFactory(mm)
  val fifo: StreamFifoCC[Bits] = StreamFifoCC(
    dataType = Bits(8 bits),
    depth = fifoDepth,
    pushClock = ClockDomain.current,
    popClock = ClockDomain.external("ext")
  )
  fifo.io.push <> bus.createAndDriveFlow(Bits(8 bits), 0).toStream
  fifo.io.pop <> sendToHost
}
