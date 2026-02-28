import yfinance as yf
from datetime import datetime
from dateutil.relativedelta import relativedelta
import os

file = open("../src/stockname.txt","r+")

now = datetime.today()
six_months = datetime.today() + relativedelta(months=-6)

symbol = file.read().strip()

start_date = six_months.strftime("%Y-%m-%d")
#print (start_date)
end_date = now.strftime("%Y-%m-%d")
#print (end_date)

stock = yf.Ticker(symbol)
historical_data = stock.history(start=start_date, end=end_date)
historical_data = historical_data.drop(columns=["Volume","Dividends","Stock Splits"])
historical_data.index = historical_data.index.tz_localize(None)
historical_data.index = historical_data.index.map(lambda x: int(x.timestamp())) #string date to epoch
historical_data.to_csv(f'../src/{symbol}.csv')

file.seek(0)
file.truncate(0)
file.close()