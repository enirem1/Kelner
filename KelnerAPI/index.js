require("dotenv").config();
const express = require("express");
const mysql = require("mysql2");

const cors = require("cors");

const app = express();
app.use(express.json());
app.use(cors());
/*

http://localhost:5000/article/        
http://localhost:5000/Show/Orders        
http://localhost:5000/tables/order/        
http://localhost:5000/article/        
http://localhost:5000/article/1       
http://localhost:5000/article/ingredients/1    
http://localhost:5000/article/allergies/2   
http://localhost:5000/category
http://localhost:5000/Orders                

*/

const db = mysql.createConnection({
  host: "localhost",
  user: "root",
  password: "",
  database: "Kelner",
});
//huinq
db.connect((err) => {
  if (err) {
    console.error("Database connection failed: ", err);
  } else {
    console.log("Connected to MySQL");
  }
});

app.get("/", (req, res) => {
  res.send("Node.js API with WAMP is Running!");
});




app.get("/article/:id", (req, res) => {
  const userId = req.params.id
  db.query("SELECT * FROM article WHERE id=?", [userId], (err, result) => {
    if (err) {
      res.status(500).json({ error: err.message });
    } else {
      res.json(result);
    }
  });
});
app.get("/article/", (req, res) => {

  db.query("SELECT * FROM article ", (err, result) => {
    if (err) {
      res.status(500).json({ error: err.message });
    } else {
      res.json(result);
    }
  });
});
app.get("/category", (req, res) => {

  db.query("SELECT id,name FROM category ", (err, result) => {
    if (err) {
      res.status(500).json({ error: err.message });
    } else {
      res.json(result);
    }
  });
});

app.get("/category/articles/:id", (req, res) => {
  const aId = req.params.id;
  db.query("SELECT article.id,article.name,article.price,article.weight from article  join article_category on article.id=article_category.id_article join category on article_category.id_category=category.id WHERE category.id=? or parent_id=?", [aId, aId], (err, result) => {
    if (err) {
      res.status(500).json({ error: err.message });
    } else {
      res.json(result);
    }
  });
});
app.get("/ac/cp/:id", (req, res) => {
  const cpId = req.params.id;
  db.query("SELECT article.name as article,category.name as category from article  join article_category on article.id=article_category.id_article join category on article_category.id_category=category.id WHERE category.parent_id=?", [cpId], (err, result) => {
    if (err) {
      res.status(500).json({ error: err.message });
    } else {
      res.json(result);
    }
  });
});

app.get("/article/ingredients/:id", (req, res) => {
  const cpId = req.params.id;
  db.query(`
          SELECT i.name from ingredients as i
          join article_ingredient as ai on i.id=ai.id_ingredient
          where ai.id_article=?`, [cpId], (err, result) => {
    if (err) {
      res.status(500).json({ error: err.message });
    } else {
      res.json(result);
    }
  });
});
app.get("/article/allergies/:id", (req, res) => {
  const cpId = req.params.id;
  db.query(`
         SELECT  al.name from article as ar
          left join article_ingredient as ai on ai.id_article=ar.id
          left join ingredients as i on i.id=ai.id_ingredient  
          left join ingredients_allergies as air on air.id_ingredient=i.id
          left join allergies as al on al.id=air.id_allergie
          where ar.id=2 and al.name IS NOT NULL`, [cpId], (err, result) => {
    if (err) {
      res.status(500).json({ error: err.message });
    } else {
      res.json(result);
    }
  });
});

app.use(express.json());

app.post("/Orders", (req, res) => {
  const { id_table, id_article,status } = req.body; 

  const sql = "INSERT INTO orders (id_table, id_article,status) VALUES (?, ?, ?)";
  const values = [id_table, id_article,status];

  db.query(sql, values, (err, result) => {
    if (err) {
      return res.status(500).json({ error: err.message });
    }
    res.status(201).json({ message: "Order added"});
  });
});
app.get("/Show/Orders", (req, res) => {
  const aId = req.params.id;
  db.query(`SELECT o.Id as OrderID,t.id as TableId,a.name as Name,a.price,a.weight FROM orders as o
join article as  a on a.id=o.id_article
LEFT JOIN ttable as t on t.id=o.id_table;`, (err, result) => {
    if (err) {
      res.status(500).json({ error: err.message });
    } else {
      res.json(result);
    }
  });
});
app.delete("/tables/order/:id", (req, res) => {
  const orderId = req.params.id;
  db.query(`DELETE FROM orders WHERE id =?`, [orderId],(err, result) =>{
    if (err) {
      res.status(500).json({ error: err.message });
    } else {
      res.json(result);
    }
  });
});



const PORT = process.env.PORT || 5000;

const HOST = "0.0.0.0"; 

app.listen(PORT, HOST, () => {
  console.log(`Server running at http://${HOST}:${PORT}`);
});
