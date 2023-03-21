# FuelDisp
display fuel left for me at work

![chip](https://github.com/megatontech/FuelDisp/blob/23374e29fb5c31f43c4c6b2fef721ce7a6bea413/1880428119.jpg)
works with stm32 chip 


Parts list:
Arduino yun
X9C102
connection wires
plastic case
Fule meter
battery
DC-DC power board
optional: GPS module


Logic:
when fuel meter powered with 9~12v dc 
it works and checks resistor value,and display how much fuel remains
and we can change resistor vaule by digital signal
so we can change fule display value with digital signals


Coding:
working time starts by 09:30 ends at 18:00
we can calculate how many minutes we need to work in total
and devide them into cycles
we set each cycle a value mapped to the resistor we got a fully functional fuel clock,that reminds you need to get home early and refuel yourself.

special logic before work time fuel need to be at zero 
when recovery from black out fuel need to re-calc to current val
X9C102 need to clear current storage everytime cycle is broke
