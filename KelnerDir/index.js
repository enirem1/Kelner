require("dotenv").config();
const express = require("express");
const mysql = require("mysql2");

const cors = require("cors");

const app = express();
app.use(express.json());
app.use(cors());
/*
http://localhost:5000/
http://localhost:5000/ac          --select article and category 
http://localhost:5000/ac/a/1    --select category and article by article id  
http://localhost:5000/ac/c/1   --select category and article by category id  
http://localhost:5000/ac/cp/1   --select category and article by category paernt_id  
http://localhost:5000/article/    --select all
http://localhost:5000/article/1   --select by id
http://localhost:5000/category/1

*/
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
  const userId=req.params.id
    db.query("SELECT * FROM article WHERE id=?",[userId], (err, result) => {
      if (err) {
        res.status(500).json({ error: err.message });
      } else {
        res.json(result);
      }
    });
  });
app.get("/category/:id", (req, res) => {
  const userId=req.params.id
    db.query("SELECT * FROM category WHERE id=?",[userId], (err, result) => {
      if (err) {
        res.status(500).json({ error: err.message });
      } else {
        res.json(result);
      }
    });
  });
  app.get("/ac/", (req, res) => {
    
      db.query("SELECT article.name as article,category.name as category from article  join article_category on article.id=article_category.id_article join category on article_category.id_category=category.id ", (err, result) => {
        if (err) {
          res.status(500).json({ error: err.message });
        } else {
          res.json(result);
        }
      });
    });
app.get("/ac/a/:id", (req, res) => {
    const aId=req.params.id;
      db.query("SELECT article.name as article,category.name as category from article  join article_category on article.id=article_category.id_article join category on article_category.id_category=category.id WHERE article.id=?",[aId], (err, result) => {
        if (err) {
          res.status(500).json({ error: err.message });
        } else {
          res.json(result);
        }
      });
    });
    app.get("/ac/c/:id", (req, res) => {
      const aId=req.params.id;
        db.query("SELECT article.name as article,category.name as category from article  join article_category on article.id=article_category.id_article join category on article_category.id_category=category.id WHERE category.id=?",[aId], (err, result) => {
          if (err) {
            res.status(500).json({ error: err.message });
          } else {
            res.json(result);
          }
        });
      });
      app.get("/ac/cp/:id", (req, res) => {
        const cpId=req.params.id;
          db.query("SELECT article.name as article,category.name as category from article  join article_category on article.id=article_category.id_article join category on article_category.id_category=category.id WHERE category.parent_id=?",[cpId], (err, result) => {
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
