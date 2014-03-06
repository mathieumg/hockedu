-- ----------------------------
-- Table structure for [comm_log]
-- ----------------------------
CREATE TABLE [dbo].[comm_log2] (
  [id] int NOT NULL,
  [user_id] int NOT NULL,
  [type] int NOT NULL,
  [data] text NOT NULL,
  [ipadress] text NOT NULL,
  [time] int NOT NULL,
  PRIMARY KEY ([id])
);

-- ----------------------------
-- Table structure for [users]
-- ----------------------------
CREATE TABLE [dbo].[users] (
  [id] int NOT NULL,
  [username] text NOT NULL,
  [password] text NOT NULL,
  [password_salt] text NOT NULL,
  [email] text NOT NULL,
  [registration_time] int NOT NULL,
  PRIMARY KEY ([id])
);

-- ----------------------------
-- Table structure for [maps]
-- ----------------------------
CREATE TABLE [dbo].[maps] (
  [id] int NOT NULL ,
  [user_id] int NOT NULL,
  [name] text NOT NULL,
  [description] text NOT NULL,
  [content] text NOT NULL,
  [cache_name] text NOT NULL,
  [is_public] int NOT NULL,
  [creation_time] int NOT NULL,
  [rating_average] int DEFAULT NULL,
  [rating_count] int DEFAULT NULL,
  [last_modified_time] int NOT NULL,
  PRIMARY KEY ([id])
);

-- ----------------------------
-- Table structure for [achievement_progress]
-- ----------------------------
CREATE TABLE [dbo].[achievement_progress] (
  [id] int NOT NULL,
  [id_user] int NOT NULL,
  [id_achievement] int NOT NULL,
  [unlocked] int NOT NULL,
  [progress_data] text NOT NULL,
  [time] int NOT NULL,
  PRIMARY KEY ([id])
);

-- ----------------------------
-- Table structure for [achievements_list]
-- ----------------------------
CREATE TABLE [dbo].[achievements_list] (
  [id] int NOT NULL,
  [name] text NOT NULL,
  [description] text NOT NULL,
  PRIMARY KEY ([id])
);

INSERT INTO [achievements_list] ([name],[description])
VALUES  ('Break the ice', 'Start the game'),
		('Welcome to the victor''s club', 'Win a game'),
        ('Keep going Jack', 'Win 5 games'),
	    ('Way to go bro', 'Win 20 games'),
		('You like that game too much', 'Win 50 games'),
		('Ok now just go play outside', 'Win 100 games'),
		('Need some friends', 'Create 3 AI profiles'),
		('Need more friends', 'Create 10 AI profiles'),
		('I''m really forever alone', 'Create 20 AI profile'),
		('Stopping noise pollution', 'Mute the game'),
		('Welcome to Aperture Science', 'Create a portal'),
		('I suppose you want some cake?', 'Create 10 portals'),
		('Now you''re thinking with portals', 'Create 20 portals'),
		('The cake is a lie','Create 50 portals');

-- ----------------------------
-- Table structure for [matches]
-- ----------------------------
CREATE TABLE [dbo].[matches] (
  [id] int NOT NULL,
  [id_user1] int NOT NULL,
  [id_user2] int NOT NULL,
  [score_user1] int NOT NULL,
  [score_user2] int NOT NULL,
  [time] int NOT NULL,
  PRIMARY KEY ([id])
);

-- ----------------------------
-- Table structure for [remote_authentication]
-- ----------------------------
CREATE TABLE [dbo].[remote_authentication] (
  [id] int NOT NULL ,
  [key] text NOT NULL,
  [id_user] int NOT NULL,
  [creation] int NOT NULL,
  [expiration] int NOT NULL,
  PRIMARY KEY ([id])
);