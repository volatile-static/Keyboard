package keyboard

import spinal.core._
import spinal.lib._

case class ResetController(powerOnTime: TimeNumber = 1 ms,
                           externalTime: TimeNumber = 3 sec
                          ) extends Component {
  val io = new Bundle{
    val clock: Bool = in Bool
    val externalReset: Bool = in Bool
    val globalReset: Bool = out Bool
  }
  val freq: HertzNumber = ClockDomain.current.frequency.getValue

  new ClockingArea(new ClockDomain(
    clock = io.clock,
    config = ClockDomainConfig(resetKind = BOOT)
  )) {
    val cnt1: Counter = Counter((freq * powerOnTime).toBigInt)
    val por: Bool = RegNextWhen(False, cnt1.willOverflowIfInc, True)
    when(por) (cnt1.increment)

    val cnt2: Counter = Counter((freq * externalTime).toBigInt)
    when(io.externalReset) (cnt2.increment)
    when(io.externalReset.rise(False)) (cnt2.clear)

    io.globalReset := RegNext(por || cnt2.willOverflow)
  }
}
