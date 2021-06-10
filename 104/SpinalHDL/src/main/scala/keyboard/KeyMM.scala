package keyboard

import spinal.core._
import spinal.lib._
import spinal.lib.bus.avalon._
import spinal.lib.eda.altera._

case class KeyMM(fifoDepth : Int = 8) extends Component {
  val io = new Bundle {
    noIoPrefix()
    val scanIdx: Flow[Bits] = slave Flow Bits(8 bits)
    val s1, s2 = slave(AvalonMM(
      AvalonMMSlaveFactory.getAvalonConfig(3, 32)))
    val irq1, irq2 = out(Reg(Bool).init(false))
  }

  val keyFlow: Flow[Bits] = Flow(Bits(8 bits))
  keyFlow.valid := io.scanIdx.valid.rise
  keyFlow.payload := io.scanIdx.payload
  val keyStream: Stream[Bits] = keyFlow.m2sPipe.toStream(
    overflow = False.allowOverride,
    fifoSize = fifoDepth,
    overflowOccupancyAt = fifoDepth
  )
  keyStream.ready := ~(io.irq1 | io.irq2)

  def mmNios(avalon : AvalonMM, irq : Bool): Area = new Area {
    avalon addTag ClockDomainTag(ClockDomain.current)
    irq addTag InterruptSenderTag(ClockDomain.current)

    val bus: AvalonMMSlaveFactory = AvalonMMSlaveFactory(avalon)
    bus.read(keyStream.payload, 0)

    when(keyStream.valid.rise(False)) {
      irq := True
    }
    when(avalon.read) {
      irq := False
    }
  }
  mmNios(io.s1, io.irq2)
  mmNios(io.s2, io.irq1)
}
