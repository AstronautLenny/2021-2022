from smbus import SMBus

arduino_1 = 0x08 #bus addresses
arduino_2 = 0x09
bus = SMBus(1)

print("[ArduinoNum (1 or 2)] [SolenoidNum (0 or 1)] [FiringDuration (Sec)]")
#solenoidNum: 0/1 corresponds to top/bottom or left/right
#duration: can only send up to 255 for byte data

def fireSolenoid(aNum, sNum, duration):
#     a = aNum;
    if aNum == 1:
        a = arduino_1
    elif aNum == 2:
        a = arduino_2        
    bus.write_byte_data(a, int(sNum), int(duration)) #send state for first LED
    
fireSolenoid(1, 0, 2);
fireSolenoid(1, 1, 5);

# while True:
#     try:
#         aNum, sNum, duration = input(">>  ").split() #entering the ardunio and states for each LED
#         if((aNum == "1" or aNum == "2") and (sNum == "0" or sNum == "1") and (int(duration) >= 0 and int(duration) <= 255)):
#             fireSolenoid(aNum, sNum, duration);            
#         else:
#             print("Incorrect Values")
#     except ValueError: #catch value error in case user clicks enter with no input     
#         break    
#     input("Press return twice to exit, once to go again")
#     
    