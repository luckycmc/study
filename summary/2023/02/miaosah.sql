/*
Navicat MySQL Data Transfer
Source Server         : 01 本地localhost
Source Server Version : 50553
Source Host           : localhost:3306
Source Database       : test
Target Server Type    : MYSQL
Target Server Version : 50553
File Encoding         : 65001
Date: 2020-11-06 14:31:35
*/
 
SET FOREIGN_KEY_CHECKS=0;
 
-- ----------------------------
-- Table structure for products
-- ----------------------------
DROP TABLE IF EXISTS `products`;
CREATE TABLE `products` (
  `id` int(10) NOT NULL AUTO_INCREMENT COMMENT 'ID',
  `title` varchar(50) DEFAULT NULL COMMENT '货品名称',
  `store` int(11) DEFAULT '0' COMMENT '货品库存',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COMMENT='货品表';
 
-- ----------------------------
-- Records of products
-- ----------------------------
INSERT INTO `products` VALUES ('1', '稻花香大米', '20');
 
-- ----------------------------
-- Table structure for order_log
-- ----------------------------
DROP TABLE IF EXISTS `order_log`;
CREATE TABLE `order_log` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `content` varchar(255) CHARACTER SET utf8mb4 DEFAULT NULL COMMENT '日志内容',
  `c_time` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '创建时间',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
 
-- ----------------------------
-- Table structure for order
-- ----------------------------
DROP TABLE IF EXISTS `order`;
CREATE TABLE `order` (
  `oid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '订单号',
  `product_id` int(11) DEFAULT '0' COMMENT '商品ID',
  `number` int(11) DEFAULT '0' COMMENT '购买数量',
  `c_time` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '创建时间',
  PRIMARY KEY (`oid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='订单表';