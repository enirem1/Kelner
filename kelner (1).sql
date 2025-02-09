-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1:3306
-- Generation Time: Feb 09, 2025 at 04:21 PM
-- Server version: 9.1.0
-- PHP Version: 8.3.14

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
) ENGINE=MyISAM AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `allergies`
--

INSERT INTO `allergies` (`id`, `name`) VALUES
(1, 'nuts'),
(2, 'diary'),
(3, 'gluten');

-- --------------------------------------------------------

--
-- Table structure for table `article`
--

DROP TABLE IF EXISTS `article`;
CREATE TABLE IF NOT EXISTS `article` (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `price` decimal(10,2) NOT NULL,
  PRIMARY KEY (`id`)
) ;

--
-- Dumping data for table `article`
--

INSERT INTO `article` (`id`, `name`, `price`) VALUES
(1, 'Margherita', 10.99),
(2, 'Pepperoni', 12.99),
(3, 'Coca-Cola', 2.50);

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
(1, 3),
(2, 4);

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
(2, 1),
(2, 4);

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
) ENGINE=MyISAM AUTO_INCREMENT=8 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `category`
--

INSERT INTO `category` (`id`, `name`, `parent_id`) VALUES
(1, 'Main', NULL),
(2, 'Drinks', NULL),
(3, 'Pizza', 1),
(4, 'Large Pizza', 3),
(5, 'Soda', 2),
(7, 'Juices', 2);

-- --------------------------------------------------------

--
-- Table structure for table `ingredients`
--

DROP TABLE IF EXISTS `ingredients`;
CREATE TABLE IF NOT EXISTS `ingredients` (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=7 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `ingredients`
--

INSERT INTO `ingredients` (`id`, `name`) VALUES
(1, 'Cheese'),
(2, 'Tomato'),
(3, 'Lettuce'),
(4, 'Beef'),
(5, 'dough'),
(6, 'cheesse');

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
(5, 3),
(6, 2);

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
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
-- Dumping data for table `orders`
--

INSERT INTO `orders` (`id`, `id_table`, `id_article`, `status`) VALUES
(1, 1, 2, NULL),
(2, 2, 3, NULL);

-- --------------------------------------------------------

--
-- Table structure for table `ttable`
--

DROP TABLE IF EXISTS `ttable`;
CREATE TABLE IF NOT EXISTS `ttable` (
  `id` int NOT NULL AUTO_INCREMENT,
  `total_Price` decimal(10,2) DEFAULT '0.00',
  PRIMARY KEY (`id`)
) ;

--
-- Dumping data for table `ttable`
--

INSERT INTO `ttable` (`id`, `total_Price`) VALUES
(1, 18.98),
(2, 10.99);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
