# Necessary imports
import serial
import requests
import os
import time
import RPi.GPIO as GPIO
from dotenv import load_dotenv

load_dotenv

# GPIO Config
TX_PIN = 14
RX_PIN = 15

GPIO.setmode(GPIO.BCM)

# Set up radio receive
SERIAL_PORT = "/dev/ttyS0"
TOKEN = os.getenv('TOKEN')
BAUD_RATE = 115200
RECEIVER_ADDRESS = 2

# You will need to generate an API key from Pushbullet and put it in a .env file.
# It is not recommended to hardcode your API key.
# Your API key becomes your TOKEN reference.

# Set up pushbullet
PUSHBULLET_URL = "https://api.pushbullet.com/v2/pushes"

HEADERS = {
    "Content-Type":"application/json",
    #"Access-Token": TOKEN
    "Authorization": f"Bearer {TOKEN}"
}

# Define Pushbullet notification function
def send_pushbullet_notification(message):
    payload = {
        "type":"note",
        "title":"Water Monitor",
        "body": message,
    }
    try:
        response = requests.post(PUSHBULLET_URL, json=payload, headers=HEADERS)
        print("Pushbullet Response:", response.text)
        print("Status code:", response.status_code)
        print("Response: ", response.json())
    except Exception as e:
        print("Request failed:", e)


def main():
    # Serial print for debugging
    print("SENDING PUSH BULLET NOTIFICATION");
    
    with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1) as ser:
        # Setup radio with AT command by talking over serial port
        ser.write(bytes(F"AT+ADDRESS={RECEIVER_ADDRESS}\r\n", 'ascii'))
        while True:
            try:
                line = ser.readline().decode("ascii").strip()
                if line:
                    print("Received:", line)
                    
			        # Modifying message for better formatting
				    if line.startswith(“+RCV=”):
					parts = line.split(‘,’)
					If len(parts) >= 3:
						Message = parts[2]
						print(“Message:”, message)
                    			send_pushbullet_notification(message)
			    time.sleep(2.5) # Prevents spamming
            except Exception as e:
                print("Error:", e)


if __name__ == "__main__":
    main()