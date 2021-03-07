package keyboard

import spinal.core._
import spinal.lib._
import spinal.lib.com.uart.Uart

case class kbd104() extends BlackBox {
  val clk_clk: Bool = in Bool()
  val reset_reset_n: Bool = in Bool()
  val uart: Uart = master(Uart())
  val io_row: Bits = out Bits(6 bits)
  val io_col: Bits = out Bits(35 bits)
  val flash: Bundle = new Bundle {
    val dclk, sce, sdo = out Bool
    val data0: Bool = in Bool
  }
//  mapCurrentClockDomain(
//    clock = clk_clk,
//    reset = reset_reset_n,
//    resetActiveLevel = LOW)
}
