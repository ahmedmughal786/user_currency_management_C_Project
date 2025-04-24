---

### 🎯 **Goal of the Program:**
This program reads user data (name, amount, and currency) from a file called `coins.txt`, processes it, and allows the user to:

1. **View user information**  
2. **Save the user data into a CSV file**  
3. **Exit the program**

---

### 📂 **Input:**
- A file named `coins.txt` that contains:
  - Name
  - Amount
  - Currency (US$, AU$, or Euro)

---

### 🧠 **How it Works:**

#### **1. Load Data**
- Open the file `coins.txt`
- Read the total number of user records
- For each line, extract:
  - Name
  - Amount (must be between 1 and 95)
  - Currency (must be US$, AU$, or Euro)

#### **2. Store Data**
- If the user already exists, update their currency amount
- If it's a new user, add them to the users array

#### **3. Menu**
- Show a menu with 3 choices:
  1. View user info
  2. Save data to CSV
  3. Exit

#### **4. View User Info**
- Ask for the user's name
- If found, display all amounts for each currency

#### **5. Save to CSV**
- Save each user's name and amounts in US$, AU$, and Euro to a file called `user_data.csv`

#### **6. (Optional) Change Calculation**
- Although it's declared, you also have some logic ready to calculate and display change in denominations like 50, 20, 10, etc.

---

### 💡 **Functions Summary:**

- **loadUserData()**: Loads all data from the file  
- **processLine()**: Parses each line and validates it  
- **addNewUser() / updateUserAmount()**: Adds or updates user data  
- **displayUserDetails()**: Shows user’s currency amounts  
- **saveChangeToCSV()**: Saves all data into a CSV file  

---
