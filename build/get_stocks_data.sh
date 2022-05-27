#echo Fetching FAANG stocks from the last 20 years
name=1;
for name in "$@" 
do

wget "https://www.alphavantage.co/query?function=TIME_SERIES_MONTHLY_ADJUSTED&symbol=$name&apikey=UK4XWXM1BV4OQLVE" -O $name.stock -q
#echo Fetched $name monthly
sleep 13s
wget "https://www.alphavantage.co/query?function=EARNINGS&symbol=$name&apikey=UK4XWXM1BV4OQLVE&format=json" -O $name.esp -q
#echo Fetched $name esp
sleep 13s

name=$((name+1));
done
