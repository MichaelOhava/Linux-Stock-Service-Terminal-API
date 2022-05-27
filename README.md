# Linux-Stock-Service-Terminal-API
This is a simple command API for the Linux terminal to download and display stock information on-screen.
Using a Master procces, and a Slave process, displaying simple Inter Process Communication, this small but powerful API downloads stock data, displayes it-onscreen 
and also has the ability to zip all of the data into a .zip (using ziplib)
data is being processed using json-master repository.


# User installation instructions:
1) Download files into desired folder as is. <br />
2) Open the "build" folder  in terminal.  <br />
4) Write : "cmake .." in terminal. <br />
5) Write : "make" in terminal. <br />
6) Write : "./run_stocks_api" in terminal.  <br />

# How to use API:
This API has 4 simple functions to be used in the terminal: <br />
1) Downloading and processing desired stock from the internet (specifically Alphavantage) - enter stock name like "FB" and wait for it to appear in folder.
2) Display existing .data in folder - showing what stock data is available in folder.
3) Displaying in the terminal the .data information you've downloaded - choose stock name and it will show in terminal
4) Zip all of .data stock files in directory into a zip file.
