#!/usr/bin/env python3

from jnpr.junos import Device
from jnpr_interface_info import GetInterfaceInformation

dev=None
hostname="192.168.101.31"
junos_username="lab"
junos_password="lab123"

def dev_open(conn_info):
    print('DevOpen')
    try:
        dev = Device(**conn_info)
        dev.open()
        dev.timeout = 1800
    except RuntimeError as err:
        print(f"[{conn_info['host']}] Open failed: Runtime Error")
    except ConnectAuthError as err:
        print(f"[{conn_info['host']}] Open failed: Connect Auth Error")
    except ConnectError as err:
        print(f"[{conn_info['host']}] Open failed: Cannot connect to device")
    except Exception as err:
        print(f"[{conn_info['host']}] Device open failed")
        print(f"[{conn_info['host']}] Device open failed: ")
        


def dev_close():
    print('DevClose')
    try:
        if dev:
            dev.close()
    except:
        print('Device close failed')
        
        
        

dev_open({'host':hostname, 'user':junos_username, 'passwd':junos_password})

try:
    eths_rpc = GetInterfaceInformation(dev).get() 
except Exception as err:
    print("Get eths rpc error: "+str(err))

dev_close()
