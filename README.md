High Frequency Trading & 2010 Market Flash Crash
==============================

### Analysis Jupyter Notebook
* **Market Flashcrash:** [financial-data-notebook](../master/financial-data-notebook.ipynb)
-----

#### Introduction

As computational capabilities continue to grow exponentially and financial markets around the world are increasingly dependable on automated systems it is worth to spend some time analysis the market depth data of the weeks leading to and the day of May 6, 2010 to have a better understanding of market behavior during this high volatility period and to compare some of this project findings with official report to congress from the CFTC and SEC. The data used in this project is from CME Group and corresponds to market depth transactions in milliseconds of the E-Mini S\&P 500 futures and options contracts.

#### Methodology

The market depth data that the CME Group provides contains all market data messages required to recreate the order book (list of orders that a trading firm uses to record the interest of buyers and sellers in a particular financial instrument.) each message contains between five to ten orders deep in futures markets and three orders deep in options markets this data is time stamped to the millisecond allowing for an in depth analysis of the price movement.

In order to process the large volume of transactions (millions of transactions per week) and the goal is to compute daily, hourly, minute, seconds, millisecond volume and other data metrics it is necessary to implement statistical operations such as distributions, average in parallel as well as filtering map-reduce type jobs are ideal for type of task as the nature of the data (independent transactions) allows for parallel processing in most cases.


## Additional Resources

* [Findings Regarding The Market Events of May 6, 2010](../master/01-resources/marketevents-report.pdf)

* [The Flash Crash: The Impact of High Frequency Trading on an Electronic Market](../master/01-resources/oce_flashcrash0314.pdf)

