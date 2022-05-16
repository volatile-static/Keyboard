package keyboard

import spinal.core._
import spinal.lib._
import spinal.lib.bus.avalon._
import spinal.lib.eda.altera._

case class KeyMM(fifoDepth : Int = 8) extends Component {
  val keyStatus: Vec[Flow[Bool]] = Vec(Flow(Bool), 103)
  val io = new Bundle {
    noIoPrefix()
    val scanIdx: Flow[Bits] = slave Flow Bits(8 bits)
    val s1, s2 = slave(AvalonMM(
      AvalonMMSlaveFactory.getAvalonConfig(9, 32)))
    val irq1, irq2 = out(Reg(Bool).init(false))
    val keyBits: Bits = in(Bits(keyStatus.asBits.getBitsWidth bits))
    keyStatus.assignFromBits(keyBits)
  }

  val keyFlow: Flow[Bits] = Flow(Bits(8 bits))
  keyFlow.valid := io.scanIdx.valid.rise
  keyFlow.payload := io.scanIdx.payload
  val keyStream: Stream[Bits] = keyFlow.m2sPipe.toStream(
    overflow = False.allowOverride,
    fifoSize = fifoDepth,
    overflowOccupancyAt = fifoDepth
  )
  keyStream.ready := ~io.irq1 | ~io.irq2 // ~(io.irq1 | io.irq2)

  def mmNios(avalon : AvalonMM, irq : Bool): Area = new Area {
    avalon addTag ClockDomainTag(ClockDomain.current)
    irq addTag InterruptTag(ClockDomain.current, avalon)

    val bus: AvalonMMSlaveFactory = AvalonMMSlaveFactory(avalon)
    bus.read(keyStream.payload, 0)
    for (i <- keyStatus.indices)
      bus.readStreamNonBlocking(keyStatus(i).toStream(False), i*4 + 4, 0, 1)

    when(keyStream.valid) {
      irq := True
    }
    when(avalon.read) {
      irq := False
    }
  }
  mmNios(io.s1, io.irq1)
  mmNios(io.s2, io.irq2)
}
