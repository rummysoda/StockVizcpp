import yfinance as yf
from datetime import datetime
from dateutil.relativedelta import relativedelta
import pandas as pd
from tabulate import tabulate
import os

file = open("src\stockname.txt","r+")

now = datetime.today()
six_months = datetime.today() + relativedelta(months=-6)

symbol = file.read()
start_date = six_months.strftime("%Y-%m-%d")
print (start_date)
end_date = now.strftime("%Y-%m-%d")
print (end_date)

stock = yf.Ticker(symbol)
historical_data = stock.history(start=start_date, end=end_date)
historical_data = historical_data.drop(columns=["Volume","Dividends","Stock Splits"])

historical_data.to_csv(f'src/{symbol}.csv')


file.truncate(0)