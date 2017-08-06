-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.0.24a-community-nt - MySQL Community Edition (GPL)
-- Server OS:                    Win32
-- HeidiSQL Version:             8.3.0.4694
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

-- Dumping database structure for final
CREATE DATABASE IF NOT EXISTS `final` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `final`;


-- Dumping structure for table final.widget
CREATE TABLE IF NOT EXISTS `widget` (
  `widgetid` bigint(20) unsigned NOT NULL auto_increment,
  `color` tinytext,
  `style` tinytext,
  `quantity` bigint(20) default NULL,
  PRIMARY KEY  (`widgetid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Dumping data for table final.widget: ~12 rows (approximately)
/*!40000 ALTER TABLE `widget` DISABLE KEYS */;
INSERT INTO `widget` (`widgetid`, `color`, `style`, `quantity`) VALUES
	(31, 'red', 'economy', 5),
	(32, 'yellow', 'standard', 3),
	(33, 'green', 'advanced', 7),
	(34, 'blue', 'exceptional', 8);
/*!40000 ALTER TABLE `widget` ENABLE KEYS */;
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
