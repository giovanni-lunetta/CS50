# export API_KEY=pk_4eb5956ba0bf48969e6a619e0c858343

import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

from datetime import datetime, timezone

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response
# -----------------------------------------------------------------------------------------------------------------------------------------------\


@app.route("/")
@login_required
def index():
    user_id = session["user_id"]

    stocks = db.execute("SELECT symbol, price, SUM(shares) as shares FROM transactions WHERE user_id = ? GROUP BY symbol", user_id)
    cash_SQL = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    cash = cash_SQL[0]["cash"]

    #total = cash

    # for stock in stocks:
    # total += stock["price"] * stock["shares"]

    return render_template("index.html", stocks=stocks, cash=cash)
# ------------------------------------------------------------------------------------------------------------------------------------------------

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "GET":
        return render_template("buy.html")

    # Grab the two variables needed from the html file.
    # Change shares to an int so we can use math.
    symbol = request.form.get("symbol")
    shares = int(request.form.get("shares"))

    # If user doesnt enter a symbol show error.
    if symbol == "":
        return apology("Must enter a stock tikker.")

    # Calling the lookup function just like before.
    stock_result = lookup(symbol)

    # If the stock does not exist give error.
    if not stock_result:
        return apology("There is no stock with this tikker.")

    # If they enter shares less than 0 give error.
    if shares <= 0:
        return apology("Shares must exceed 0.")

    # Define how to calculate the cost of the stock
    cost = shares * stock_result["price"]

    # Check where the current logged in user is and grab there user_id along with the cash that they have.
    user_id = session["user_id"]
    cash_SQL = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    cash = cash_SQL[0]['cash']

    # If the price of the shares is more than the amount of money the user has return an error.
    if cash < cost:
        return apology("Purchase failed. Insufficient funds")

    # Simple math to get the remaining cash.
    remaining_cash = cash - cost

    # Update the database by inputting in the remaining cash they have.
    db.execute("UPDATE users SET cash = ? WHERE id = ?", remaining_cash, user_id)

    # Because I know the variables I am going to need to use in the db.execute, I am defining them here.
    # Then I am running db.execute to actually insert the correct information into the database.
    symbol = stock_result["symbol"]
    price = stock_result["price"]
    date = datetime.now()

    db.execute("INSERT INTO transactions (user_id, symbol, shares, price, date) VALUES(?, ?, ?, ?, ?)", user_id, symbol, shares, price, date)

    # Flash that the purchase was successful.
    flash("Your purchase was successful")

    return redirect("/")
# ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

@app.route("/history")
@login_required
def history():
    user_id = session["user_id"]
    rows =  db.execute("SELECT symbol, shares, price, date FROM transactions WHERE user_id = ?", user_id)
    return render_template("history.html", rows = rows)
# ------------------------------------------------------------------------------------------------------------------------------------------

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")
#------------------------------------------------------------------------------------------------------

@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")
# -----------------------------------------------------------------------------------------

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "GET":
        return render_template("quote.html")

    # Get this from html file
    symbol = request.form.get("symbol")

    # If user doesn't input anything return apology.
    if not symbol:
        return apology("Must enter a stock tikker.")

    # Using the given function in helpers to access database of all stocks
    stock_result = lookup(symbol)

    # If the stock tikker entered is non existent return apology
    if not stock_result:
        return apology("There is no stock with this tikker.")

    # Return the html with proper tags.
    return render_template("quoted.html", name = stock_result["name"], price = stock_result["price"], symbol = stock_result["symbol"])
#-------------------------------------------------------------------------------------------------------------------------------------

@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "GET":
        return render_template("register.html")

    # define variables and grab them from html file
    username = request.form.get("username")
    password = request.form.get("password")
    confirmation = request.form.get("confirmation")

    # If the user leaves the tab blank return apology.
    if username == "":
        return apology("Please enter your username.")

    # Check the database to see if the username alreaedy exists. If the number of usernames from the database exceeds 0 then we know there is a duplicate.
    if len(db.execute('SELECT username FROM users WHERE username = ?', username)) > 0:
        return apology("Username already exists.")

    # If the user leaves the tab blank return apology.
    if password == "":
        return apology("Please enter your username.")

    # If the password and the confirmation password do not match let the user know.
    if password != confirmation:
        return apology("Passwords do not match.")

    # Hash uses the imported library to change the user password to a hash password
    hash = generate_password_hash(password)

    # Then insert the username and the hash password into the database.
    db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, hash)

    # Check the database for the username
    rows = db.execute("SELECT * FROM users WHERE username = ?", username)

    # Keep them logged in.
    session["user_id"] = rows[0]["id"]

    return redirect("/")
#-----------------------------------------------------------------------------------------------------------------------------------------

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "POST":
        user_id = session["user_id"]

        # Grab the two variables needed from the html file.
        # Change shares to an int so we can use math.
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))

        # If they enter shares less than 0 give error.
        if shares <= 0:
            return apology("Shares must exceed 0.")

        # If user doesnt enter a symbol show error.
        if symbol == None:
            return apology("Must select a stock tikker.")

        # Calling the lookup function just like before.
        stock_result = lookup(symbol)

        # Define how to calculate the cost of the stock
        cost = shares * stock_result["price"]

        # Here we use SQL to pick out the number of shares that was requested by the user of that specific stock.
        requested_shares_SQL = db.execute("SELECT shares FROM transactions WHERE user_id = ? AND symbol = ? GROUP BY symbol", user_id, symbol)
        requested_shares = requested_shares_SQL[0]["shares"]

        # Then, we compare and see if the amount that the user is trying to sell is more than what they have, which would give an error.
        if shares < requested_shares:
            return apology("You do not own this many shares. Please select the number of shares you would like to sell and try again.")

        # Check where the current logged in user is and grab there user_id along with the cash that they have.
        cash_SQL = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        cash = cash_SQL[0]['cash']

        # Simple math to get the new total.
        remaining_cash = cash + cost

        # Update the database by inputting in the remaining cash they have.
        db.execute("UPDATE users SET cash = ? WHERE id = ?", remaining_cash, user_id)

        # Because I know the variables I am going to need to use in the db.execute, I am defining them here.
        # Then I am running db.execute to actually insert the correct information into the database.
        symbol = stock_result["symbol"]
        price = stock_result["price"]
        shares = (-1)*shares
        date = datetime.now()

        db.execute("INSERT INTO transactions (user_id, symbol, shares, price, date) VALUES(?, ?, ?, ?, ?)", user_id, symbol, shares, price, date)

        # Flash that the purchase was successful.
        flash("You sold your shares successfully.")

        return redirect("/")

    else:
        user_id = session["user_id"]
        symbols = db.execute("SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0", user_id)
        return render_template("sell.html", symbols = symbols)
# ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------