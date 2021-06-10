package keyboard

import spinal.core._
import spinal.lib._

case class TPK(clk: ClockDomain) extends BlackBox {
  val clk_clk: Bool = in Bool()
  val reset_reset_n: Bool = in Bool()
  val led_row_export: Bits = out Bits(6 bits)
  val led_col_export: Bits = out Bits(35 bits)
  val scan_idx: Flow[Bits] = slave Flow Bits(8 bits)
  val send_to_host: Stream[Bits] = master Stream Bits(8 bits)
  val receive_from_host: Stream[Bits] = slave Stream Bits(8 bits)
  val locked_export: Bool = out Bool()
  mapClockDomain(
    clockDomain = clk,
    clock = clk_clk,
    reset = reset_reset_n,
    resetActiveLevel = LOW)
}
