package keyboard

import spinal.core._
import spinal.lib._
import spinal.lib.com.uart._

object FlowToUart {
  def apply(scanIdx: Flow[Bits]): Bool = {
    val uartCtrl = new UartCtrl
    uartCtrl.io.config.setClockDivider(115200 Hz)
    uartCtrl.io.config.frame.dataLength := 7  //8 bits
    uartCtrl.io.config.frame.parity := UartParityType.NONE
    uartCtrl.io.config.frame.stop := UartStopType.ONE
    uartCtrl.io.writeBreak := False
    uartCtrl.io.write.payload := scanIdx.toReg
    val valid = RegInit(False)
    uartCtrl.io.write.valid := valid
    when(scanIdx.valid) {
      valid := True
    } elsewhen uartCtrl.io.write.ready {
      valid := False
    }

    uartCtrl.io.uart.rxd := True
    uartCtrl.io.uart.txd
  }
}
