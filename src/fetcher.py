import yfinance as yf
from datetime import datetime
from dateutil.relativedelta import relativedelta
import pandas as pd
from tabulate import tabulate

now = datetime.today()
six_months = datetime.today() + relativedelta(months=-6)

symbol = 'AMZN'
start_date = six_months.strftime("%Y-%m-%d")
print (start_date)
end_date = now.strftime("%Y-%m-%d")
print (end_date)

stock = yf.Ticker(symbol)
historical_data = stock.history(start=start_date, end=end_date)

pd.set_option('display.max_columns', None)
pd.set_option('display.width', None)

print(f"Historical Data for {symbol} from {start_date} to {end_date}")

formatted_data = pd.concat([historical_data.head(), historical_data.tail()])
print(tabulate(formatted_data, headers='keys', tablefmt='psql'))

print("\nShowing only the first and last 5 rows of data:")
print(tabulate(formatted_data, headers='keys', tablefmt='grid'))