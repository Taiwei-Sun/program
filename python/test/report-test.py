from datetime import datetime, date
from sunnybank.database import schema, tools
import uuid
import json






def get_log(start_time=None, end_time=None, log_type='F5'):
    logs = list()
    global data

    start_time="03/12/2017"
    end_time="03/13/2017"
    if start_time is None or end_time is None:
        today = True
    else:
        start_time = datetime.strptime(start_time, "%m/%d/%Y")
        end_time = datetime.strptime(end_time, "%m/%d/%Y")
        today = False

        if end_time < start_time:
            today = True

    with tools.init_session() as session:
        if today:
            print 'Get today data.'
            all_rows = session.query(schema.LogData).filter_by(log_type=log_type).filter(schema.LogData.date_time >= date.today())
            

        else:
            print 'Get time range data.'
            all_rows = session.query(schema.LogData).filter_by(log_type=log_type).filter(schema.LogData.date_time.between(start_time, end_time)).filter(schema.LogData.detail.find("PPP")!=-1)

        for row in all_rows:
            logs.append(row.get_dict())
    
    print logs
    index = uuid.uuid1().hex
    #data[index] = json.dumps(logs)
    #print json.dumps(logs)

    return index



print get_log()
