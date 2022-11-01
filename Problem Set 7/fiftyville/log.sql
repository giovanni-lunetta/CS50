-- Keep a log of any SQL queries you execute as you solve the mystery.

--This is the line of code from the video that looks into the crime scene reports.
SELECT description FROM crime_scene_reports WHERE month = 7 AND day = 28 AND street = "Humphrey Street";
--| Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery. |
--| Littering took place at 16:36. No known witnesses.

--This leads me to look at the activty from the bakery logs at around 10:00 and the liscence plates as well, none of this was enough information to find anything therefore I need to take a step back.
SELECT activity FROM bakery_security_logs WHERE month = 7 AND day = 28 AND hour = 10 AND year = 2021;
SELECT license_plate FROM bakery_security_logs WHERE month = 7 AND day = 28 AND hour = 10 AND year = 2021;

--My next train of though is to look into interviews
SELECT name FROM interviews WHERE month = 7 AND day = 28 AND year = 2021;

--Having the same person interview twice seems suspicious so I am going to check what he said.
--Eugene said "I don't know the thief's name, but it was someone I recognized.
--Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money."
--Important facts are the street name, and the fact that the money was being withdrawn.
SELECT name, transcript FROM interviews WHERE month = 7 AND day = 28 AND year = 2021 AND name = "Eugene";

--Now that I have some traction I need to look around at this street name and atm. Using .tables again I see there is an atm_transactions table so this is what I am going to look into.
--This gave me 8 account numbers and the amount that was withdrawn.
SELECT account_number, amount FROM atm_transactions WHERE atm_location = 'Leggett Street' AND transaction_type = 'withdraw' AND year = 2021 AND month = 7 AND day = 28;

--Now I have to find the people who have actually own the accounts by joining the tables where names and account numbers are located.
--First we join people and bank accounts because they both contain the names of the people. Then we join their atm transactions from their account numbers that are located in the account number table and the transaction table.
SELECT name, atm_transactions.amount FROM people JOIN bank_accounts ON people.id = bank_accounts.person_id JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number WHERE atm_transactions.atm_location = 'Leggett Street' AND atm_transactions.transaction_type = 'withdraw' AND atm_transactions.year = 2021 AND atm_transactions.month = 7 AND atm_transactions.day = 28;

--Now we have 8 suspects. They are Bruce, Diana, Brooke, Kenny, Iman, Luca, Taylor, Benista.
--Now I am stuck because I do not have any more info other than 8 witnesses. Looking back on what I did I probably should have looked at every witnesses transcipts to see if they had anything to say so i am going to do that again.
SELECT name, transcript FROM interviews WHERE month = 7 AND day = 28 AND year = 2021 AND name != "Eugene";

--Ruth seems to have given some good info that I probably should have looked at before so I am going to use my initial knowledge again but now with more info. (Raymond gave info to just to remember for later)
--We know that the activity was the person was exiting, we know the time stamp, and we want to look at anyone 15 to 25 minutes.
--Also, I have now finally realized that I should probably make my code look nice instead of making it extremely long. That is why this now is formatted well.
SELECT name, bakery_security_logs.hour, bakery_security_logs.minute
  FROM people
  JOIN bakery_security_logs
    ON people.license_plate = bakery_security_logs.license_plate
WHERE bakery_security_logs.month = 7
   AND bakery_security_logs.day = 28
   AND bakery_security_logs.year = 2021
   AND bakery_security_logs.hour = 10
   AND bakery_security_logs.activity = 'exit'
   AND bakery_security_logs.minute >= 15
   AND bakery_security_logs.minute <= 25
 ORDER BY bakery_security_logs.minute;

--This gives us another 8 names: Vanessa, Bruce, Barry, Luca, Sofia, Iman, Diana and Kelsey
-- ---------------------------   Bruce, Diana, Brooke, Kenny, Iman, Luca, Taylor, Benista.
-- Names that are the same: Bruce, Luca, Iman and Diana. These are the only 4 suspects now.
--Now lets look again at what Raymond said.
--"As the thief was leaving the bakery, they called someone who talked to them for less than a minute.
--In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
--The thief then asked the person on the other end of the phone to purchase the flight ticket.

--The first obvious stick outs are that the phone call lasted less than a minute, earliest flight, someone else purchased.
--First we find that the airport located in Fiftyville is abbreviated CSF.
SELECT abbreviation, full_name, city FROM airports WHERE city = 'Fiftyville';

--Of course, flights and airports are not the same table so we are going to need to join them where the origin airport is both fiftyville.
--Now we have a list of all the flights on the day after the theft leaving CSF airport and the earliest is 8:20 am.
SELECT flights.id, full_name, city, flights.hour, flights.minute
  FROM airports
  JOIN flights
    ON airports.id = flights.origin_airport_id
WHERE abbreviation = 'CSF'
   AND flights.year = 2021
   AND flights.month = 7
   AND flights.day = 29
 ORDER BY flights.hour, flights.minute;

--Now we must find where this flight is going.
--And we can use the same logic as above and we find out that it is going to New York.
SELECT flights.id, full_name, city, flights.hour, flights.minute
  FROM airports
  JOIN flights
    ON airports.id = flights.destination_airport_id
 WHERE flights.id = 36
   AND flights.year = 2021
   AND flights.month = 7
   AND flights.day = 29
 ORDER BY flights.hour, flights.minute;

 --We should be pretty close now. Now we just have to find out who was on that plane ride and cross reference it with the 4 suspsects we got from the other info.
 --This should be easy because we just have to join the tables by id number as we know the plane id is 36 and the peoples names.
 SELECT passengers.flight_id, name, passengers.passport_number, passengers.seat
  FROM people
  JOIN flights
    ON passengers.flight_id = flights.id
  JOIN passengers
    ON people.passport_number = passengers.passport_number
 WHERE flights.month = 7
   AND flights.day = 29
   AND flights.hour = 8
   AND flights.minute = 20
   AND flights.year = 2021
 ORDER BY passengers.passport_number;

 --Now lets cross reference from our original list: Bruce, Luca, Iman and Diana.
 --The names that are the same are Bruce and Luca (ofc it has to be 2 :( ), therefore one of these two are the one who commited the crime.
 --Thankfully raymond gave us more info, he said that the call lasted less than 60 seconds so lets look at all the calls that were less than 60 seconds.
 SELECT phone_calls.id, name, phone_calls.duration
  FROM people
  JOIN phone_calls
    ON people.phone_number = phone_calls.caller
 WHERE phone_calls.duration <= 60
   AND phone_calls.month = 7
   AND phone_calls.day = 28
   AND phone_calls.year = 2021
 ORDER BY phone_calls.duration;

 --This narrows down our list to one person and one person only BRUCE. BRUCE is the theft.
 --Now we just hvae to find out who he called because that person will be the accomplice.
 SELECT name FROM people JOIN phone_calls ON people.phone_number = phone_calls.receiver WHERE phone_calls.id = 233;

 --And just like that Bruce is the theif, and Robin , the accomplice purchased the ticket for him to go to NYC.