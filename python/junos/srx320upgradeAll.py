import os, sys, logging
from jnpr.junos import Device
from jnpr.junos.utils.sw import SW
from jnpr.junos.exception import ConnectError


from jnpr.junos.utils.config import Config
from lxml import etree
import xml.etree.ElementTree as ET
import time
import os
import threading
'''
systemctl start xinetd
systemctl start tftp
systemctl start dhcpd

'''
packageOS = '/var/lib/tftpboot/junos-srxsme-15.1X49-D180.2-domestic.tgz'
#packageOS = '/var/lib/tftpboot/junos-srxsme-15.1X49-D170.4-domestic.tgz'
packageFirmware = '/var/lib/tftpboot/jfirmware-srxsme-15.1X49-D180.2-signed.tgz'
#packageFirmware = '/var/lib/tftpboot/jfirmware-srxsme-15.1X49-D140.2-signed.tgz'
remote_path = '/var/tmp'
local_path='./log/'
validate = True
junos_username="lab"
junos_password="lab123"
rebootTimeout=600
hostnet="172.18.1."
dhcpStart=100
dhcpNetmask=24
checkDevicesLog=local_path+'ok.log'

def showLog(msg):
    print msg
    logging.info(msg)

def check_ping(hostname):
    response = os.system("ping -c 1 " + hostname)
    # and then check the response...
    if response == 0:
        return True
    else:
        return False



class MyThread(threading.Thread):
    hostIP=None
    status="None"
    serialNumber=None
    waiteTime=0
    def run(self):
        self.hostIP=hostnet+self.getName()
        self.status="Start"
        
        update_process(type="checkSerialNumber",host=self.hostIP,package=None,threadIns=self)
        #update_process(type="setFixIP",host=self.hostIP,package=None,threadIns=self)
        #update_process(type="OS",host=self.hostIP,package=packageOS,threadIns=self)
        update_process(type="Firmware",host=self.hostIP,package=packageFirmware,threadIns=self)
        
        update_process(type="Snapshot",host=self.hostIP,package=None,threadIns=self)
        
        update_process(type="createLogFile",host=self.hostIP,package=None,threadIns=self)
        
        self.status="done"
        
        
def update_progress(dev, report):
    # log the progress of the installing process
    logging.info(report)

def create_log(dev):

    serialNumber=dev.facts['serialnumber']
    logFile=local_path+serialNumber+".log"
    print "create log:"+logFile
    f = open(logFile, "a")
    
    
    #show system alarm|no-more #get-system-alarm-information
    f.write("show system alarm|no-more")
    f.write(etree.tostring(dev.rpc.get_system_alarm_information({'format':'text'})))
    #show chassis alarm|no-more #get-alarm-information
    f.write("show chassis alarm|no-more")
    f.write(etree.tostring(dev.rpc.get_alarm_information({'format':'text'})))
    #show chassis environment|no-more #get-environment-information
    f.write("show chassis environment|no-more")
    f.write(etree.tostring(dev.rpc.get_environment_information({'format':'text'})))
    #show chassis routing-engine|no-more #get-route-engine-information
    f.write("show chassis routing-engine|no-more")
    f.write(etree.tostring(dev.rpc.get_route_engine_information({'format':'text'})))
    #show chassis hardware|no-more #get-chassis-inventory
    f.write("show chassis hardware|no-more")
    f.write(etree.tostring(dev.rpc.get_chassis_inventory({'format':'text'})))
    #show chassis fpc|no-more #get-fpc-information
    f.write("show chassis fpc|no-more")
    f.write(etree.tostring(dev.rpc.get_fpc_information({'format':'text'})))
    #show chassis fpc pic-status|no-more #get-pic-information
    f.write("show chassis fpc pic-status|no-more")
    f.write(etree.tostring(dev.rpc.get_pic_information({'format':'text'})))
    #show system license|no-more #get-license-summary-information
    f.write("show system license|no-more")
    f.write(etree.tostring(dev.rpc.get_license_summary_information({'format':'text'})))
    #show version #get-software-information
    f.write("show version")
    f.write(etree.tostring(dev.rpc.get_software_information({'format':'text'})))
    #show system snapshot media internal |no-more #get-snapshot-information <media>internal</media>
    f.write("show system snapshot media internal |no-more")
    f.write(etree.tostring(dev.rpc.get_snapshot_information({'format':'text'},media='internal')))
    #show modem wireless network cl-1/0/0|no-more #get-modem-wireless-network <interface-name>cl-1/0/0</interface-name>
    f.write("show modem wireless network cl-1/0/0|no-more")
    f.write(etree.tostring(dev.rpc.get_modem_wireless_network({'format':'text'},interface_name='cl-1/0/0')))
    #show system firmware|no-more #get-system-firmware-information
    f.write("show system firmware|no-more")
    f.write(etree.tostring(dev.rpc.get_system_firmware_information({'format':'text'})))
    #show modem wireless profiles cl-1/0/0 slot 1 #get-modem-wireless-profiles <slot>1</slot> <interface-name>cl-1/0/0</interface-name>
    f.write("show modem wireless profiles cl-1/0/0 slot 1")
    f.write(etree.tostring(dev.rpc.get_modem_wireless_profiles({'format':'text'},slot='1',interface_name='cl-1/0/0')))

    f.close()
    print logFile+" finished"
    
    
    f = open(checkDevicesLog, "a")
    f.write(serialNumber)
    f.close()

def update_process(type,host,package,threadIns):
    
    # verify package exists
    if type=='OS' or type=='Firmware':
        if not (os.path.isfile(package)):
            msg = 'Software package does not exist: {0}. '.format(package)
            logging.error(msg)
            print msg
            sys.exit()
    time.sleep(20)
    dev=None
    dev = Device(host=host,user=junos_username, passwd=junos_password)
    rebootTime=10
    rebootOK=False
    while not rebootOK:
        try:
            time.sleep(10)
            rebootOK=True
            dev.open()
        except Exception as err:
            print threadIns.hostIP+threadIns.serialNumber+':wait device time='+str(rebootTime)+"Sec  "+str(err)
            logging.info(threadIns.hostIP+threadIns.serialNumber+':wait device time='+str(rebootTime)+"Sec  "+str(err))
            rebootTime+=10
            rebootOK=False

        if rebootTime>=rebootTimeout:
            logging.error('Cannot connect to device\n')
            print 'Cannot connect to device\n'
            return
    
        



    
        
    if type=='createLogFile':
        threadIns.status="createLog"
        dev.timeout = 100
        create_log(dev)
        dev.close()
        return
        
    if type=='checkSerialNumber':
        threadIns.status="checkSN"
        threadIns.serialNumber=dev.facts['serialnumber']
        dev.close()
        return
    
    if type=='Snapshot':
        threadIns.status="Snapshot"
        dev.timeout = 600
        dev.rpc.request_snapshot(slice='alternate')
        dev.close()
        return
    
    if type=='setFixIP':
        threadIns.status="setFixIP"
        cu = Config(dev)
        cu.load('delete interfaces ge-0/0/0', format='set')
        cu.load('set interfaces ge-0/0/0 unit 0 family inet address '+threadIns.hostIP+'/'+str(dhcpNetmask), format='set')
        cu.pdiff()
        cu.commit()
        return
    
    
    
    
    # Create an instance of SW
    sw = SW(dev)
    
    threadIns.status="install"+type
    try:
        logging.info('Starting the software upgrade process: {0}'.format(package))
        okOS = sw.install(package=package, remote_path=remote_path,progress=update_progress, validate=validate)
        #okOS=True
    except Exception as err:
        msg = 'Unable to install software, {0}'.format(err)
        logging.error(msg)
        okOS = False
    
    
    if type=="Firmware":
        #UPGRADED SUCCESSFULLY
        logging.info("!!!!!!!!!!!!!")
        logging.info(dev.cli("request system firmware upgrade pic pic-slot 0 fpc-slot 1"))
        threadIns.status="checkFirmware"
        
        okFirmware=False
        #okFirmware=True
        firmwareTime=0
        
        while not okFirmware and okOS:
            xmlStr=etree.tostring(dev.rpc.get_system_firmware_information(), encoding='unicode')
            xmlRoot = ET.fromstring(xmlStr)
            for firmware_status in xmlRoot.iter('firmware-status'):
                #UPGRADED SUCCESSFULLY
                if firmware_status.text=='UPGRADED SUCCESSFULLY':okFirmware=True
                if firmware_status.text!='OK':print firmware_status.text
            print threadIns.hostIP+threadIns.serialNumber+":wate firmware upgrade "+str(firmwareTime)+" Sec"
            time.sleep(1)
            firmwareTime=firmwareTime+1
            #print okFirmware
    elif type=="OS":okFirmware=True
    
    
    
    threadIns.status="reboot after install"+type
    if okOS is True and okFirmware is True:
        logging.info('Software installation complete. Rebooting')
        rsp = sw.reboot()
        logging.info('Upgrade pending reboot cycle, please be patient.')
        logging.info(rsp)
    else:
        msg = 'Unable to install software, {0}'.format(okOS)
        logging.error(msg)
    
    # End the NETCONF session and close the connection
    dev.close()
    
def showThreadStatus(ths):
    titleStr=['None','Start','checkSN','setFixIP','installOS','reboot after installOS','installFirmware','checkFirmware','reboot after installFirmware','Snapshot','createLogFile','done']

    if len(ths)>0:
        for i in range(0,100):
            for j in titleStr:
                print j+": ",
                for k in ths:
                    if k.status==j:print k.serialNumber,
                print "   |"
            time.sleep(10)
        
def showThreadStatusOnes(ths):
    titleStr=['None','Start','checkSN','setFixIP','installOS','reboot after installOS','installFirmware','checkFirmware','reboot after installFirmware','Snapshot','createLogFile','done']

    if len(ths)>0:
        for j in titleStr:
            print j+": ",
            for k in ths:
                if k.status==j:print k.serialNumber,
            print "   |"

def main():
    
    try:  
        os.mkdir(local_path)
    except OSError as err:  
        print ("Creation of the directory {} failed ".format(local_path)+str(err))
    else:  
        print ("Successfully created the directory {} ".format(local_path))
    
    bug_log=local_path+'junos-pyez-install.log'
    # initialize logging
    logging.basicConfig(filename=bug_log, level=logging.INFO,format='%(asctime)s:%(name)s: %(message)s')
    logging.getLogger().name = "srx320-upgrade"
    logging.getLogger().addHandler(logging.StreamHandler())#when run this python, terminals can show log
    logging.info('Information logged in {0}'.format(bug_log))

    #f = open(checkDevicesLog, "w")
    #f.write("Devices OK:\n")
    #f.close()

    nextIP=dhcpStart
    ths=[]
    
    for i in range(0,3):
        while not check_ping(hostnet+str(nextIP)):
            print 'next ip: '+hostnet+str(nextIP)
            showThreadStatusOnes(ths)
            time.sleep(10)
        ths.append(MyThread(name =str(nextIP)))
        ths[i].start()
        nextIP+=1
    
    
    #threading.Thread(target=showThreadStatus,args=(ths,)).start()
    
    




if __name__ == "__main__":
    main()


