package keyboard

import spinal.core._

case class reset_controller() extends BlackBox {
  val clock, KEY_Fn: Bool = in Bool()
  val reset: Bool = out Bool()
}
