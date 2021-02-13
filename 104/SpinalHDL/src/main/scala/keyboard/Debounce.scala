package keyboard

import spinal.core._
import spinal.lib._

case class Debounce(scanPeriod: TimeNumber, bounceTime: TimeNumber) extends Component {
  val key_down: Bits = in Bits(103 bits)
  val keyStatus: Vec[Flow[Bool]] = Vec(master(Flow(Bool)), 103)

  case class SingleKey(cntInit: BigInt) extends Component {
    val raw: Bool = in Bool
    val filtered: Flow[Bool] = master Flow Bool

    val cnt: UInt = RegInit(cntInit)
    when(raw.edge) { cnt := U(cntInit) }
    when(cnt =/= 0) (cnt := cnt - 1)
    filtered.valid := RegNext(cnt === 1)
    filtered.payload := RegNextWhen(!raw, cnt === 1)
//    println(s"latency ${LatencyAnalysis(filtered.valid, filtered.payload)}\n")
  }
  new SlowArea(scanPeriod.toHertz, true) {
    for (idx <- 0 until 103) {
      val debounceOne = SingleKey(1 + (bounceTime / scanPeriod).toBigInt)
      debounceOne.raw <> key_down(idx)
      debounceOne.filtered <> keyStatus(idx)
    }
  }
}
