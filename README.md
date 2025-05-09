# STM32F4-Discovery-Board
### Here I have put some Examples for STM32F4-Discovery-Board.

**Description:**
- Board: STM32F407G-DISC1 (old name: STM32F4DISCOVERY) [link: https://www.st.com/en/evaluation-tools/stm32f4discovery.html]  
- MCU on the Board: STM32F407VGT6
- for Projects with Atollic TrueSTUDIO: main file is saved at location '<project-name>\src\main.c'  
- for Projects with STM32CubeIDE: main file is saved at location '<project-name>\Core\Src\main.c'  

### Projects and IDE used:

| S.No. | Project                           | IDE/Tools Used          |
| ----- | --------------------------------- | ----------------------- |
| 1     | LED_Blink                         | Atollic TrueSTUDIO      |
| 2     | RNG                               | Atollic TrueSTUDIO      |
| 3     | EXTI_Interrupt_PushButton_OnBoard | Atollic TrueSTUDIO      |
| 4     | Timer_TIM10                       | Atollic TrueSTUDIO      |
| 5     | USART                             | Atollic TrueSTUDIO      |
| 6     | ADC_onboardTemperatureSensor      | Atollic TrueSTUDIO      |
| 7     | ADC_OnBoard3VPinRead              | STM32CubeIDE            |
| 8     | IIC_Master                        | STM32CubeIDE            |
| 9     | IIC_Slave                         | STM32CubeIDE            |
| 10    | CAN                               | Atollic TrueSTUDIO      |
| 11    | USB_Device_CDC                    | STM32CubeIDE            |
| 12    | LED_Blink_CMake                   | STM32CubeIDE, and CMake |

#### USB_Device_CDC
- USB Port (micro-USB) available on the discovery board

#### LED_Blink_CMake
!!! See *./LED_Blink_CMake/README.md* for documentation.

### STM32F4-Discovery Board
![STM32F4-Discovery Board](zz_docs/STM32F4-Discovery.jpg)
