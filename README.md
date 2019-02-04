# 4 Player Pong (avancerat projekt i kursen IS1200 på KTH)
Av Erik Flink och Robin Holmquist

## Mål och krav
Målet med projektet var att utveckla en modifierad version av det klassiska spelet Pong, där det i stället för två spelare är fyra spelare (en längs varje sida av skärmen). Spelarna styr sina paddlar för att hindra en boll från att lämna spelplanen på sin egen sida, samt för att få den att åka ut på en motståndares sida. De huvudsakliga måste-kraven för projektet var:
- Spelarna ska kunna kontrollera sina paddlar med hjälp av glidpotentiometrar som ansluts via de analoga GPIO-portarna.
- Spelet ska visas på en extern skärm som är ansluten via I2C (därmed avancerat projekt).
- Spelet ska ha olika svårighetsgrader: Bollens starthastighet ska variera beroende på svårighetsgrad.
- De olika spelarnas poäng ska visas i realtid.

Extra funktioner som skulle kunna implementeras:
- Poängvisning på sjusegments-displayer.
- Paus-funktion.
- En startmeny.
- Enspelarläge.
## Lösning
Vi utvecklade spelet på en ChipKIT Uno32 med en Basic I/O-shield, samt använde en extern skärm (I2C) och fyra glidpotentiometrar (analoga). Den externa skärmen används för att visa spelet, och skärmen på I/O-skölden visar spelarnas poäng. Glidpotentiometrarna används för att styra spelarnas paddlar och knapparna på I/O-skölden används för att byta svårighetsgrad. För att kontrollera hastigheten på spelet används avbrott i kombination med en inbyggd timer.

Spellogiken och skärm-kommunikationen kodades i C. Vidare används pic32mx.h biblioteket, och kompileringen gjordes med hjälp av MCB32tools. För versionshantering av koden användes GitHub.

Som grund för projektet användes kod skriven av Axel Isaksson och Fredrik Lundevall. För I2Ckommunikationen användes viss kod skriven av Isaksson som en mall. Koden för kommunikationen med skärmen på IO-skölden är i princip helt skriven av Isaksson. För att göra det möjligt med avbrott, samt för att avbrotten ska hanteras utan att orsaka problem och hoppa till rätt funktion, används en av Holmquist modifierad assembly-fil skriven av Isaksson och Lundevall.

När programmet startar körs först ett antal initierings-funktioner. Dessa ställer in inställningar för en timer, I2C-kommunikation, den inbyggda skärmen, knapparna samt ADC-kretsen som börjar skanna igenom de analoga portar som används för avläsning av glidpotentiometrarna. I detta stadie av exekveringen initialiseras även alla de variabler som används för spellogiken. Varje spelare börjar med fem (5) poäng, svårighetsgraden sätts till den lägsta (1) av fyra olika (1 - 4) och bollen placeras i mitten av skärmen.

Med hjälp av timern sker ett avbrott 15 gånger per sekund. Vid varje avbrott kallas uppdateringsfunktioner som tar in data från glidpotentiometrarna, uppdaterar spellogiken och skickar lämplig data till skärmarna. Bollen förflyttar sig olika långt beroende på svårighetsgraden. Om bollen träffar någon av spelarnas paddlar byter den riktning (beroende på var på paddeln bollen träffar). Ifall bollen passerar någon av spelarnas paddel förlorar den spelaren ett (1) poäng. Då någon av spelarna förlorat alla sina poäng avslutas spelet, I2C-skärmen släcks och I/O-sköldens skärm visar vilken spelare som förlorade.

Vid varje avbrott kollar också programmet om någon av I/O-sköldens knappar tryckts ned. I sådana fall startar spelet om och svårighetsgraden ställs in beroende på vilken knapp som tryckts ned.

## Verifiering
Genom projektets gång testades programmet kontinuerligt, på hårdvaran, för att verifiera om målen nåtts, samt för att hitta eventuella buggar. Holmquist och Flink läste även igenom varandras kod för att hitta eventuella fel som inte upptäckts vid kompilering.

## Bidrag
Hårt draget skrev Flink koden för kommunikation med skärmarna samt glidpotentiometrarna och Holmquist skrev kod för själva spellogiken. Vad gäller den analoga kommunikationen skrev Holmquist först en prototyp-kod för manuell avläsning och omvandling av de analoga signalerna. Detta ändrades sedan av Flink så att ADC-kretsen själv skannar igenom de analoga portarna.

Mot slutet av projektet blev Flink även aktiv i spellogikskoden för att Holmquist och Flink på ett bra sätt skulle kunna anpassa den till skärmkommunikationen.

## Reflektioner
Resultatet av projektet var ett fungerande pong-spel som uppfyllde alla de måste-krav som ställdes. Projektet var därmed lyckat. På grund av tidsbrist hanns inte implementation av de föreslagna extra funktionerna med.

Samarbetet har fungerat bra, och vi är båda nöjda med projektet. Att vi bara hade tillgång till en MIPS-processor gjorde det lite svårt för oss att arbeta hemifrån samtidigt, i och med att det bara var en person i taget som kunde kontrollera om koden verkligen gjorde det man ville att den skulle göra. Vi lyckades dock lösa det på ett bra sätt genom att vi uppehöll regelbunden kommunikation.

GitHub är ett väldigt intressant och effektivt verktyg som vi använt oss av i stor omfattning under utvecklingsprocessen. Att lära sig mer om hur Git fungerar har varit en värdefull del av projektet.
