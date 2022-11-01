# export API_KEY=pk_4eb5956ba0bf48969e6a619e0c858343
# flask run --host=0.0.0.0 --port=8080
import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required

from datetime import datetime, timezone

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///wallet.db")

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
def welcome():
    user_id = session["user_id"]

    return render_template("welcome.html")
# ------------------------------------------------------------------------------------------------------------------------------------------------

@app.route("/add", methods=["GET", "POST"])
@login_required
def add():
    if request.method == "GET":
        return render_template("add.html")

    # Grab the four variables needed from the html file.
    # Change price to an int so we can use math.
    type = request.form.get("type")
    grade = int(request.form.get("grade"))
    date = request.form.get("date")
    price = int(request.form.get("price"))
    name = request.form.get("name")

    # If user doesnt enter a type show error.
    if type == "":
        return apology("Must enter a type of product to add to wallet.")

    if name == "":
        return apology("Must enter a type of product to add to wallet.")

    # If they enter a grade less than 0 give error.
    if grade < 0:
        return apology("Grade must be above 0.")

    if price < 0:
        return apology("Price of the product must be greater than 0.")

    user_id = session["user_id"]

    db.execute("INSERT INTO transactions (user_id, type, grade, price, date, name) VALUES(?, ?, ?, ?, ?, ?)", user_id, type, grade, price, date, name)
    out = db.execute("SELECT value FROM users WHERE id = ?", user_id)
    print(out)
    value = out[0]["value"]
    #NOT WORKING
    # value = price
    new_value = value + price

    db.execute("UPDATE users SET value = ? WHERE id = ?", new_value, user_id)
    # Flash that the purchase was successful.
    flash("Your added to your wallet!")

    return redirect("/")
# ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

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
@app.route("/sale", methods=["GET", "POST"])
@login_required
def sale():
    if request.method == "POST":
        user_id = session["user_id"]
        # data = request.get_json()
        id = request.form.get("id")
        # print(name)
        # print(name)
        user_info = db.execute("SELECT * FROM transactions WHERE user_id = ? AND id = ?", user_id, id)
        # print(user_info)
        # Grab the four variables needed from the html file.
        # Change price to an inz so we can use math.
        # type = request.form.get("type")
        # grade = request.form.get("grade")
        # date = request.form.get("date")
        # price = request.form.get("price")
        # name = request.form.get("name")

        # user_id = session["user_id"]


        # rows = db.execute("SELECT * FROM users WHERE id = ?", user_id)
        # Execute a transaction
        db.execute("INSERT INTO transactions (user_id, type, grade, price, date, name) VALUES(?, ?, ?, ?, ?, ?)", user_id, user_info[0]["type"], user_info[0]["grade"], -user_info[0]["price"], user_info[0]["date"], user_info[0]["name"])


        total_value = db.execute("SELECT value FROM users WHERE id = ?", user_id)
        print(total_value)
        value = total_value[0]["value"]
        value -= user_info[0]["price"]
        db.execute("UPDATE users SET value = ? WHERE id = ?;", value, user_id)

        stock_SQL = db.execute("SELECT in_stock FROM transactions WHERE user_id = ?", user_id)
        print(stock_SQL)
        stock = stock_SQL[0]["in_stock"]
        stock = 0
        db.execute("UPDATE transactions SET in_stock = ? WHERE user_id = ?", stock, user_id)

        flash("Sold!")

        return redirect("/")

    else:
        user_id = session["user_id"]
        rows = db.execute("SELECT * FROM transactions WHERE user_id = ? AND in_stock != 0 AND price > 0", user_id)
        print(rows)
        return render_template("sale.html", name = rows)
#----------------------------------------------------------------------------------------------------------------------------
@app.route("/history")
@login_required
def history():
    user_id = session["user_id"]
    rows =  db.execute("SELECT type, price, name, grade, date FROM transactions WHERE user_id = ?", user_id)
    return render_template("history.html", rows = rows)
#-------------------------------------------------------------------------------------------------------------------------------

@app.route("/dashboard")
@login_required
def dashboard():
    user_id = session["user_id"]

    return render_template("dashboard.html")