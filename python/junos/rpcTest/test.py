#!/usr/bin/env python3

from jnpr.junos import Device
from jnpr_interface_info import GetInterfaceInformation


hostname="192.168.101.31"
junos_username="lab"
junos_password="lab123"

def dev_open(conn_info):
    print('DevOpen')
    dev=None
    try:
        dev = Device(**conn_info)
        dev.open()
        #print(dev)
        dev.timeout = 1800
    except RuntimeError as err:
        print("Open failed: Runtime Error:"+str(err))
    except ConnectAuthError as err:
        print("Open failed: Connect Auth Error:"+str(err))
    except ConnectError as err:
        print("Open failed: Cannot connect to device:"+str(err))
    except Exception as err:
        print("Device open failed")
        print("Device open failed: "+str(err))
    return dev


def dev_close(dev):
    print('DevClose')
    try:
        if dev:
            dev.close()
    except:
        print('Device close failed')
        
def show_items(items):
    for item in items:
        print(item)

dev=dev_open({'host':hostname, 'user':junos_username, 'passwd':junos_password})

"""
dev = Device(host=hostname, user=junos_username,passwd=junos_password)
try:
    dev.open()
except Exception as err:
    print("Get dev open error: "+str(err))
"""


try:
    eths_rpc = GetInterfaceInformation(dev).get()
    #print(eths_rpc)
except Exception as err:
    print("Get eths rpc error: "+str(err))

#show_items(eths_rpc)
#print(eths_rpc['ge-5/0/0'].admin_status)#origin tag 'admin-status'
print(eths_rpc['ge-5/0/0'].speed)

rpc_reply = dev.rpc.get_chassis_inventory()
print(rpc_reply)
print(dir(rpc_reply))
print(rpc_reply.__dir__())
#show_items(rpc_reply)
#doc = rpc_reply.getOwnerDocument()
#print(doc)


print(dir())


dev_close(dev)
