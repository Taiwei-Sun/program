import os, sys, logging
from jnpr.junos import Device
from jnpr.junos.utils.sw import SW
from jnpr.junos.exception import ConnectError

from lxml import etree
import xml.etree.ElementTree as ET
import time
import threading
import random 
import logging

nodeStatus=[]

def check_thread(threads):
    for i in range(0,len(threads):
        if threads[i].status!="down":return True
    return False
    
class MyThread(threading.Thread):
    status="start"
    def run(self):
        global nodeStatus
        print "thread-"+self.getName()+" Start"
        #logging.info('logging thread-'+str(threadNum)+" Start")
        tmpTime=random.randint(1, 5)
        time.sleep(tmpTime)
        self.status='init'
        tmpTime=random.randint(1, 10)
        time.sleep(tmpTime)
        self.status='config'
        tmpTime=random.randint(1, 5)
        time.sleep(tmpTime)
        self.status='start end'
        tmpTime=random.randint(1, 5)
        time.sleep(tmpTime)
        self.status='down'
        print "thread-"+self.getName()+" End"
        #logging.info('logging thread-'+str(threadNum)+" End")

def main():
    #logging.basicConfig(filename='./ttt.log', level=logging.INFO,format='%(asctime)s:%(name)s: %(message)s')
    logging.getLogger().name = 'test logging'
    #logging.getLogger().addHandler(logging.StreamHandler())
    logging.info('ttttt')

    
    th=[]
    for i in range(0,5):
        th.append(MyThread(name = str(i + 1)))
        th[i].start()
        time.sleep(1)
    
    
    while check_thread(th):
        for i in range(0,len(th)):
            print th[i].status+" "+str(i)+" ",
        print "|"
        time.sleep(3)

   

if __name__ == "__main__":
    main()




'''test 2 connect device established from client
import socket
from jnpr.junos import Device
from jnpr.junos.exception import ConnectError
from pprint import pprint

"""
  Listen on TCP port 2200 for incoming SSH session with a device running Junos OS. 
  Upon connecting, collect and print the devices facts, 
  then disconnect from that device and wait for more connections.
"""

junos_username="lab"
junos_password="lab123"

def launch_junos_proxy(client, addr):
    val = {
            'MSG-ID': None,
            'MSG-VER': None,
            'DEVICE-ID': None
            }
    msg = ''
    count = 3
    while len(msg) < 100 and count > 0:
        c = client.recv(1)
        if c == '\r':
            continue

        if c == '\n':
            count -= 1
            if msg.find(':'):
                (key, value) = msg.split(': ')
                val[key] = value
                msg = ''
        else:
            msg += c

    print('MSG %s %s %s' %
            (val['MSG-ID'], val['MSG-VER'], val['DEVICE-ID']))
    return client.fileno()


def main():
    
    PORT = 2200

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    s.bind(('', PORT))
    s.listen(5)
    print('\nListening on port %d for incoming sessions ...' % (PORT))

    sock_fd = 0    
    while True:
        client, addr = s.accept()
        print('\nGot a connection from %s:%d' % (addr[0], addr[1]))
        sock_fd = launch_junos_proxy(client, addr)

        print('Logging in ...')
        try:
            with Device(host=None,user=junos_username, passwd=junos_password, sock_fd=sock_fd) as dev:
                pprint(dev.facts)
        except ConnectError as err:
            print ("Cannot connect to device: {0}".format(err))                

if __name__ == "__main__":
    main()
'''    
    


'''test 1 connect device
import sys
from getpass import getpass
from jnpr.junos import Device
from jnpr.junos.exception import ConnectError

#ssh_config_file = "~/.ssh/config_dc"
"""
hostname = input("Device hostname: ")
junos_username = input("Junos OS username: ")
junos_password = getpass("Junos OS or SSH key password: ")
"""
hostname = "192.168.105.121"
junos_username="lab"
junos_password="lab123"


dev = Device(host=hostname, user=junos_username, passwd=junos_password)

try:
    dev.open()
except ConnectError as err:
    print ("Cannot connect to device: {0}".format(err))
    sys.exit(1)
except Exception as err:
    print (err)
    sys.exit(1)
    
    

print (dev.facts)

dev.close()
'''



    