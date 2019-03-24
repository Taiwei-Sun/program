from sunnybank.database import tools, schema
from datetime import date
import datetime

def checkRepeat(dataT):
 all_rows =list()
 with tools.init_session() as session:
  all_rows = session.query(schema.LogData).filter_by(log_type='F5').filter(schema.LogData.date_time >= ( date.today() - datetime.timedelta(days=90)
 ) )

 logs=list()

 for row in all_rows:
  logs.append(row.get_dict())

 return (dataT in logs)

all_rows =list()

with tools.init_session() as session:
 all_rows = session.query(schema.LogData).filter_by(log_type='F5').filter(schema.LogData.date_time >= '2017-01-01')

logs=list()

for row in all_rows:
 logs.append(row.get_dict())

tmp=list()

tmp=logs[0]

print checkRepeat(tmp)


