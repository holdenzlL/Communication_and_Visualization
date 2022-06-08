# Assignment

**For this assignment it is your choice to continue with perl as a backend language or you can switch to a different language of your choice. In doubt you are welcome to discuss with me.**



First please find out what the

- json text format
- xml text format

for data is.

Then we this time try to do a background program, that asks the RKI statistics (RKI: Robert Koch Institute, it collects data for the COVID-19 pandemy for Germany) for the corona data.

We are interested in "incidence numbers", that means the number of new covid-19 infections in a defined area during the past 7 days, per 100,000 inhabitants. This is the currently most important value for resitrictions.

The URL for getting the data is:

https://services7.arcgis.com/mOBPykOjAyBO2ZKk/arcgis/rest/services/RKI_Landkreisdaten/FeatureServer/0/query?where=OBJECTID=$tregion_id &outFields=OBJECTID,
    GEN,
    BEZ,
    BL,
    cases,
    deaths,
    cases_per_population,
    cases7_per_100k,
    cases7_lk,
    death7_lk,
    cases7_bl_per_100k,
    cases7_bl,
    death7_bl,last_update&outSR=4326&f=json



There $region is a placeholder for the number of the required region. E.g.: 193 is city of Karlsruhe

*You can explore this API and the region numbers here:*

*https://npgeo-corona-npgeo-de.hub.arcgis.com/datasets/917fc37a709542548cc3be077a786c17_0/geoservice?geometry=5.931%2C48.692%2C13.407%2C49.945*

*Click on some location on the map and you will see the Object_ID = region no.*

Examples for regions:

- 193: Karlsruhe Stadt
- 194: Karlsruhe Landkreis
- 202: Enzkreis
- 200: Pforzheim Stadt
- 175: Kreis Südliche Weinstraße
- 176: Rhein-Pfalz-Kreis
- 159: Frankenthal
- 205: Landkreis Breisgau-Hochschwarzwald

Tasks:

1. You can start with just creating a page showing the data. Use CSS for different colours (> 100, > 165). Maybe do a div or table for the different regions of your choice.

2. Please write the data to a xml or a json file.

3. Next step: first read the file and then only append a new value if it is on a new day.

4. Create a new page that displays the history of values for a specific region (maybe called from a button on the table of first page)

   

5. Please add a page that displays a graphical history of the values for the choosen region(s). It's up to you how you do it. You can create the graphics on the fly or you can create the graphics and buffer it on the disk and display it from the disk if nothing changed. (somehow "cached"). Another option is to use gnuplot (external, extra program that is quite popular to create plots) with a cronjob to create the graphics file and then display it on the page.

For submission please include following parts:

- Full source code
- Short explanatory report with: 
  - description of the program
  - some screenshots showing the webpages

Please always be prepared to explain your solution.



# Solution

The solution to this assignment is not open source, but the report and presentation slide will be published to my website, which will be notified later.