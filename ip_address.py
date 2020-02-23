import urllib.request
import socket 
import smtplib

from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
  
def get_Host_name_IP(): 
    try: 
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect(("8.8.8.8", 80))
        internal_ip = s.getsockname()[0]
        s.close()
        return internal_ip
    except: 
       return ("Unable to get Hostname and IP") 
  
def get_external_ip():
    try:
        external_ip = urllib.request.urlopen('https://ident.me').read().decode('utf8')
        return external_ip
    except:
        return ("couldn't get external-ip")

def send_mail(internal,external):
    email = 'csash7@gmail.com' # Your email
    password = 'IN143@HARSHA' # Your email account password
    send_to_email = 'csash7@gmail.com' # Who you are sending the message to
    message = 'Internal-IP: '+str(internal)+'\n'+ 'External-Ip: '+str(external) # The message in the email

    server = smtplib.SMTP('smtp.gmail.com', 587) # Connect to the server
    server.ehlo()
    server.starttls() # Use TLS
    server.ehlo()
    server.login(email, password) # Login to the email server
    
    multipart_message = MIMEMultipart()
    multipart_message['From']=email
    multipart_message['To']=send_to_email
    multipart_message['Subject']="IP_Address from RPi"
    multipart_message.attach(MIMEText(message, 'plain'))
    
    server.send_message(multipart_message) # Send the email
    server.quit() # Logout of the email server

internal = get_Host_name_IP()
external = get_external_ip()
send_mail(internal, external)

print (str(internal))
print(str(external))
