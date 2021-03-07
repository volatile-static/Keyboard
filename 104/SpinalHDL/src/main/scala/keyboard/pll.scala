package keyboard

import spinal.core._

case class pll() extends BlackBox {
  val inclk0: Bool = in Bool
  val c0, locked = out Bool
}
