-- ----------------------------
-- Table structure for `comm_log`
-- ----------------------------
DROP TABLE IF EXISTS `comm_log`;
CREATE TABLE `comm_log` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `type` int(11) NOT NULL,
  `data` text NOT NULL,
  `ipadress` text NOT NULL,
  `time` int(11) NOT NULL,
  PRIMARY KEY (`id`)
);

-- ----------------------------
-- Table structure for `users`
-- ----------------------------
DROP TABLE IF EXISTS `users`;
CREATE TABLE `users` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` text NOT NULL,
  `password` text NOT NULL,
  `password_salt` text NOT NULL,
  `email` text NOT NULL,
  `registration_time` int(11) NOT NULL,
  PRIMARY KEY (`id`)
);

-- ----------------------------
-- Table structure for `maps`
-- ----------------------------
DROP TABLE IF EXISTS `maps`;
CREATE TABLE `maps` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `name` text NOT NULL,
  `description` text NOT NULL,
  `content` blob NOT NULL,
  `cache_name` text NOT NULL,
  `is_public` int(11) NOT NULL,
  `creation_time` int(11) NOT NULL,
  `rating_average` int(11) DEFAULT NULL,
  `rating_count` int(11) DEFAULT NULL,
  `last_modified_time` int(11) NOT NULL,
  PRIMARY KEY (`id`)
);

-- ----------------------------
-- Table structure for `achievement_progress`
-- ----------------------------
DROP TABLE IF EXISTS `achievement_progress`;
CREATE TABLE `achievement_progress` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `id_user` int(11) NOT NULL,
  `id_achievement` int(11) NOT NULL,
  `unlocked` int(11) NOT NULL,
  `progress_data` text NOT NULL,
  `time` int(11) NOT NULL,
  PRIMARY KEY (`id`)
);

-- ----------------------------
-- Table structure for `achievements_list`
-- ----------------------------
DROP TABLE IF EXISTS `achievements_list`;
CREATE TABLE `achievements_list` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` text NOT NULL,
  `description` text NOT NULL,
  PRIMARY KEY (`id`)
);

INSERT INTO `achievements_list` (`name`,`description`)
VALUES  ('Break the ice', 'Start the game'),
		('Welcome to the victor''s club', 'Win a game'),
        ('Keep going Jack', 'Win 5 games'),
	    ('Way to go bro', 'Win 20 games'),
		("You like that game too much", 'Win 50 games'),
		("Ok now just go play outside", 'Win 100 games'),
		('Need some friends', 'Create 3 AI profiles'),
		('Need more friends', 'Create 10 AI profiles'),
		('I''m really forever alone', 'Create 20 AI profile'),
		('Stopping noise pollution', 'Mute the game'),
		('Welcome to Aperture Science', 'Create a portal'),
		('I suppose you want some cake?', 'Create 10 portals'),
		('Now you''re thinking with portals', 'Create 20 portals'),
		('The cake is a lie','Create 50 portals');

-- ----------------------------
-- Table structure for `matches`
-- ----------------------------
DROP TABLE IF EXISTS `matches`;
CREATE TABLE `matches` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `id_user1` int(11) NOT NULL,
  `id_user2` int(11) NOT NULL,
  `score_user1` int(11) NOT NULL,
  `score_user2` int(11) NOT NULL,
  `time` int(11) NOT NULL,
  PRIMARY KEY (`id`)
);

-- ----------------------------
-- Table structure for `remote_authentication`
-- ----------------------------
DROP TABLE IF EXISTS `remote_authentication`;
CREATE TABLE `remote_authentication` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `key` text NOT NULL,
  `id_user` int(11) NOT NULL,
  `creation` int(11) NOT NULL,
  `expiration` int(11) NOT NULL,
  PRIMARY KEY (`id`)
);