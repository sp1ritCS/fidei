# Listing of biblical books

In order to provide the best experience to users, Fidei expects bibles with properly labeled books. Specifically the `bsname` (here shorthand) attribute of `BIBLEBOOK` is used to save the currently active chapter to reopen after closing or after switching bible.

Should you be interesting in creating or modifying your prefered bible to work properly with Fidei, you should ensure that it's books `bsname` attibutes match the ones in the following tables.

The `bnumber` attribute (here num) is currently not used by Fidei. Fidei exclusivly relies on the order of books within the XML file, as this is faster and less memory heavy to parse. It also doesn't use the attribute to save the current state, because the shorthand is simpler for a human to identify in some kind of config/settings editor and because it ensures, that if a new bible with missing (or different) books is opened, it will show the welcome page instead of a wrong chapter.
Nontheless, I'd reccomend to also use the numbers in the table below to avoid erroneous behaviour in other software.

## Old Testament

|Full Name         |Num|Shorthand|
|------------------|---|---------|
|1 Mose/Genesis    |1  |Gen      |
|2 Mose/Exodus     |2  |Exod     |
|3 Mose/Leviticus  |3  |Lev      |
|4 Mose/Numbers    |4  |Num      |
|5 Mose/Deuteronomy|5  |Deut     |
|Joshua            |6  |Josh     |
|Judges            |7  |Judg     |
|Ruth              |8  |Ruth     |
|1 Samuel          |9  |1Sam     |
|2 Samuel          |10 |2Sam     |
|1 Kings           |11 |1Kgs     |
|2 Kings           |12 |2Kgs     |
|1 Chronicles      |13 |1Chr     |
|2 Chronicles      |14 |2Chr     |
|Ezra              |15 |Ezra     |
|Nehemiah          |16 |Neh      |
|Esther            |17 |Esth     |
|Job               |18 |Job      |
|Psalms            |19 |Ps       |
|Proverbs          |20 |Prov     |
|Ecclesiastes      |21 |Eccl     |
|Song of Solomon   |22 |Song     |
|Isaiah            |23 |Isa      |
|Jeremiah          |24 |Jer      |
|Lamentations      |25 |Lam      |
|Ezekiel           |26 |Ezek     |
|Daniel            |27 |Dan      |
|Hosea             |28 |Hos      |
|Joel              |29 |Joel     |
|Amos              |30 |Amos     |
|Obadiah           |31 |Obad     |
|Jonah             |32 |Jonah    |
|Micah             |33 |Mic      |
|Nahum             |34 |Nah      |
|Habakkuk          |35 |Hab      |
|Zephaniah         |36 |Zeph     |
|Haggai            |37 |Hag      |
|Zechariah         |38 |Zech     |
|Malachi           |39 |Mal      |

## Apocrypha

|Full Name                  |Num|Shorthand|
|---------------------------|---|---------|
|Tobit                      |80 |Tob      |
|Judith                     |81 |Jdt      |
|Additions to Esther        |82 |AddEst   |
|Wisdom of Solomon          |83 |Wis      |
|Jesus Sirach/Ecclesiasticus|84 |Sir      |
|Baruch                     |85 |Bar      |
|Epistle of Jeremiah        |86 |EpJer    |
|Additions to Daniel        |87 |AddDan   |
|Prayer of Azariah          |88 |Azar     |
|Bel and the Dragon         |89 |Bel      |
|Song of Three Youths       |90 |SgThree  |
|Susanna                    |91 |Sus      |
|1 Maccabees                |92 |1Macc    |
|2 Maccabees                |93 |2Macc    |
|3 Maccabees                |94 |3Macc    |
|4 Maccabees                |95 |4Macc    |
|1 Esdras                   |96 |1Esd     |
|2 Esdras                   |97 |2Esd     |
|Prayer of Manasseh         |98 |Man      |
|Additional Psalm           |99 |AddPs    |
|Ode                        |100|Ode      |
|Psalms of Solomon          |101|PsSol    |
|Epistle to the Laodiceans  |102|Lao      |

## New Testament

|Full Name         |Num|Shorthand|
|------------------|---|---------|
|Matthew           |40 |Matt     |
|Mark              |41 |Mark     |
|Luke              |42 |Luke     |
|John              |43 |John     |
|Acts              |44 |Acts     |
|Romans            |45 |Rom      |
|1 Corinthians     |46 |1Cor     |
|2 Corinthians     |47 |2Cor     |
|Galatians         |48 |Gal      |
|Ephesians         |49 |Eph      |
|Philippians       |50 |Phil     |
|Colossians        |51 |Col      |
|1 Thessalonians   |52 |1Thess   |
|2 Thessalonians   |53 |2Thess   |
|1 Timothy         |54 |1Tim     |
|2 Timothy         |55 |2Tim     |
|Titus             |56 |Titus    |
|Philemon          |57 |Phlm     |
|Hebrews           |58 |Heb      |
|James             |59 |Jas      |
|1 Peter           |60 |1Pet     |
|2 Peter           |61 |2Pet     |
|1 John            |62 |1John    |
|2 John            |63 |2John    |
|3 John            |64 |3John    |
|Jude              |65 |Jude     |
|Revelation        |66 |Rev      |
