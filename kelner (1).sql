-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1:3306
-- Generation Time: Feb 28, 2025 at 03:27 PM
-- Server version: 8.3.0
-- PHP Version: 8.2.18

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `kelner`
--

-- --------------------------------------------------------

--
-- Table structure for table `allergies`
--

DROP TABLE IF EXISTS `allergies`;
CREATE TABLE IF NOT EXISTS `allergies` (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `allergies`
--

INSERT INTO `allergies` (`id`, `name`) VALUES
(1, 'Nuts'),
(2, 'Dairy'),
(3, 'Gluten'),
(4, 'Seafood'),
(5, 'Soy'),
(6, 'Eggs'),
(7, 'Peanuts'),
(8, 'Shellfish'),
(9, 'Wheat'),
(10, 'Sesame');

-- --------------------------------------------------------

--
-- Table structure for table `article`
--

DROP TABLE IF EXISTS `article`;
CREATE TABLE IF NOT EXISTS `article` (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `price` decimal(10,2) NOT NULL,
  `weight` int DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=21 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `article`
--

INSERT INTO `article` (`id`, `name`, `price`, `weight`) VALUES
(1, 'Margherita Pizza', 10.99, 600),
(2, 'Pepperoni Pizza', 12.99, 750),
(3, 'Veggie Burger', 9.49, 500),
(4, 'Chicken Wrap', 8.99, 450),
(5, 'Spaghetti Bolognese', 13.50, 700),
(6, 'Caesar Salad', 7.99, 400),
(7, 'Greek Salad', 7.99, 350),
(8, 'Cheeseburger', 11.99, 600),
(9, 'BBQ Ribs', 16.99, 1000),
(10, 'Grilled Salmon', 18.99, 900),
(11, 'Coca-Cola', 2.50, 330),
(12, 'Orange Juice', 3.00, 250),
(13, 'Lemonade', 2.75, 300),
(14, 'Iced Tea', 2.99, 350),
(15, 'Espresso', 1.99, 60),
(16, 'Latte', 3.50, 250),
(17, 'Chocolate Cake', 5.99, 200),
(18, 'Cheesecake', 6.49, 220),
(19, 'Apple Pie', 4.99, 230),
(20, 'Tiramisu', 6.99, 250);

-- --------------------------------------------------------

--
-- Table structure for table `article_category`
--

DROP TABLE IF EXISTS `article_category`;
CREATE TABLE IF NOT EXISTS `article_category` (
  `id_article` int NOT NULL,
  `id_category` int NOT NULL,
  PRIMARY KEY (`id_article`,`id_category`),
  KEY `id_category` (`id_category`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `article_category`
--

INSERT INTO `article_category` (`id_article`, `id_category`) VALUES
(1, 4),
(2, 4),
(3, 5),
(6, 6),
(7, 6),
(8, 5),
(9, 8),
(10, 9),
(11, 11),
(12, 11),
(13, 11),
(14, 11),
(15, 10),
(16, 10),
(17, 12),
(18, 12),
(19, 12),
(20, 12);

-- --------------------------------------------------------

--
-- Table structure for table `article_ingredient`
--

DROP TABLE IF EXISTS `article_ingredient`;
CREATE TABLE IF NOT EXISTS `article_ingredient` (
  `id_article` int NOT NULL,
  `id_ingredient` int NOT NULL,
  PRIMARY KEY (`id_article`,`id_ingredient`),
  KEY `id_ingredient` (`id_ingredient`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `article_ingredient`
--

INSERT INTO `article_ingredient` (`id_article`, `id_ingredient`) VALUES
(1, 1),
(1, 2),
(1, 9),
(1, 10),
(2, 1),
(2, 2),
(2, 3),
(2, 9),
(3, 4),
(3, 5),
(3, 6),
(4, 5),
(4, 6),
(4, 7),
(9, 11),
(10, 12),
(17, 14),
(17, 16),
(17, 20),
(18, 13),
(18, 14),
(18, 20),
(19, 15),
(19, 18),
(19, 19);

-- --------------------------------------------------------

--
-- Table structure for table `category`
--

DROP TABLE IF EXISTS `category`;
CREATE TABLE IF NOT EXISTS `category` (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` varchar(20) NOT NULL,
  `parent_id` int DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `parent_id` (`parent_id`)
) ENGINE=MyISAM AUTO_INCREMENT=14 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `category`
--

INSERT INTO `category` (`id`, `name`, `parent_id`) VALUES
(1, 'Main Dishes', NULL),
(2, 'Drinks', NULL),
(3, 'Desserts', NULL),
(4, 'Pizza', 1),
(5, 'Burgers', 1),
(6, 'Salads', 1),
(7, 'Pasta', 1),
(8, 'Meat Dishes', 1),
(9, 'Seafood', 1),
(10, 'Hot Beverages', 2),
(11, 'Cold Beverages', 2),
(12, 'Cakes', 3),
(13, 'Pastries', 3);

-- --------------------------------------------------------

--
-- Table structure for table `ingredients`
--

DROP TABLE IF EXISTS `ingredients`;
CREATE TABLE IF NOT EXISTS `ingredients` (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=21 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `ingredients`
--

INSERT INTO `ingredients` (`id`, `name`) VALUES
(1, 'Tomato Sauce'),
(2, 'Mozzarella'),
(3, 'Pepperoni'),
(4, 'Beef Patty'),
(5, 'Lettuce'),
(6, 'Cheddar Cheese'),
(7, 'Chicken'),
(8, 'Parmesan'),
(9, 'Dough'),
(10, 'Basil'),
(11, 'BBQ Sauce'),
(12, 'Salmon'),
(13, 'Cream Cheese'),
(14, 'Eggs'),
(15, 'Wheat Flour'),
(16, 'Chocolate'),
(17, 'Coffee Beans'),
(18, 'Apple'),
(19, 'Cinnamon'),
(20, 'Vanilla Extract');

-- --------------------------------------------------------

--
-- Table structure for table `ingredients_allergies`
--

DROP TABLE IF EXISTS `ingredients_allergies`;
CREATE TABLE IF NOT EXISTS `ingredients_allergies` (
  `id_ingredient` int NOT NULL,
  `id_allergie` int NOT NULL,
  PRIMARY KEY (`id_ingredient`,`id_allergie`),
  KEY `id_allergie` (`id_allergie`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `ingredients_allergies`
--

INSERT INTO `ingredients_allergies` (`id_ingredient`, `id_allergie`) VALUES
(2, 2),
(6, 2),
(9, 3),
(13, 2),
(15, 3);

-- --------------------------------------------------------

--
-- Table structure for table `orders`
--

DROP TABLE IF EXISTS `orders`;
CREATE TABLE IF NOT EXISTS `orders` (
  `id` int NOT NULL AUTO_INCREMENT,
  `id_table` int DEFAULT NULL,
  `id_article` int DEFAULT NULL,
  `status` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `id_table` (`id_table`),
  KEY `id_article` (`id_article`)
) ENGINE=MyISAM AUTO_INCREMENT=61 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- --------------------------------------------------------

--
-- Table structure for table `ttable`
--

DROP TABLE IF EXISTS `ttable`;
CREATE TABLE IF NOT EXISTS `ttable` (
  `id` int NOT NULL AUTO_INCREMENT,
  `total_Price` decimal(10,2) DEFAULT '0.00',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `ttable`
--

INSERT INTO `ttable` (`id`, `total_Price`) VALUES
(1, 45.98),
(2, 38.49),
(3, 21.99),
(4, 30.50),
(5, 55.99),
(6, 62.75),
(7, 12.50),
(8, 8.99),
(9, 19.50),
(10, 23.99);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
