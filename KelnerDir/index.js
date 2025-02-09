require("dotenv").config();
const express = require("express");
const mysql = require("mysql2");

const cors = require("cors");

const app = express();
app.use(express.json());
app.use(cors());

// MySQL Database Connection
const db = mysql.createConnection({
  host: "localhost",
  user: "root", // Default MySQL username
  password: "", // Default WAMP has no password
  database: "Kelner",
});

db.connect((err) => {
  if (err) {
    console.error("Database connection failed: ", err);
  } else {
    console.log("Connected to MySQL");
  }
});

// Routes
app.get("/", (req, res) => {
  res.send("Node.js API with WAMP is Running!");
});

// Get all users
app.get("/article", (req, res) => {
  db.query("SELECT * FROM article", (err, result) => {
    if (err) {
      res.status(500).json({ error: err.message });
    } else {
      res.json(result);
    }
  });
});
app.get("/article/:id", (req, res) => {
    db.query("SELECT * FROM article", (err, result) => {
      if (err) {
        res.status(500).json({ error: err.message });
      } else {
        res.json(result);
      }
    });
  });
// Add a new user
app.post("/article", (req, res) => {
  const { name, price } = req.body;
  db.query(
    "INSERT INTO users (name, price ) VALUES (?, ?)",
    [name, price],
    (err, result) => {
      if (err) {
        res.status(500).json({ error: err.message });
      } else {
        res.json({ message: "Article added!", userId: result.insertId });
      }
    }
  );
});

// Start Server
const PORT = process.env.PORT || 5000;
app.listen(PORT, () => {
  console.log(`Server running on http://localhost:${PORT}`);
});
