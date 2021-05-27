DROP DATABASE IF EXISTS ChatDB;

CREATE DATABASE ChatDB;

USE ChatDB;

CREATE TABLE useres
(
    `id`          INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `email`       VARCHAR(255) DEFAULT NULL,
    `password`    VARCHAR(255) DEFAULT NULL,
    `nickname`    VARCHAR(255) DEFAULT NULL,
    `id_pictures` INT          DEFAULT NULL
);

CREATE TABLE conversations(
    `id` INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `title` VARCHAR(255) DEFAULT NULL,
    `id_creator` INT DEFAULT NULL
);

CREATE TABLE attachments(
    `id` INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `id_message` INT DEFAULT NULL,
    `url` VARCHAR(255) DEFAULT NULL
);

CREATE TABLE participants(
    `id` INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `id_conversation` INT DEFAULT NULL,
    `id_useres` INT DEFAULT NULL
);

CREATE TABLE messages(
    `id` INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `id_sender` INT DEFAULT NULL,
    `id_conversation` INT DEFAULT NULL,
    `message` VARCHAR(500) DEFAULT NULL,
    `created_at` DATETIME DEFAULT NULL,
    `deleted_at` DATETIME DEFAULT NULL,
    `isRead` TINYINT DEFAULT 0
);

ALTER TABLE useres   ADD FOREIGN KEY(id_pictures)   REFERENCES attachments(id);
ALTER TABLE conversations  ADD FOREIGN KEY(id_creator)   REFERENCES useres(id);
ALTER TABLE messages  ADD FOREIGN KEY(id_sender)   REFERENCES useres(id);
ALTER TABLE messages  ADD FOREIGN KEY(id_conversation)   REFERENCES conversations(id);


DELIMITER $$

CREATE FUNCTION HasUnreadMessages( user VARCHAR(255), other VARCHAR(255) )
RETURNS BOOL
NOT DETERMINISTIC
BEGIN
    DECLARE hasUnread BOOL;

    SELECT COUNT(m.isRead) > 0
    INTO hasUnread
    FROM messages m
             INNER JOIN useres u ON m.id_sender = u.id
             INNER JOIN participants p ON u.id = p.id_useres
             INNER JOIN conversations c ON p.id_conversation = c.id
    WHERE (m.isRead = FALSE
               AND
           ((u.nickname = user)
               AND c.id IN
                   (
                       SELECT c.id
                       FROM conversations c
                                INNER JOIN participants p ON c.id = p.id_conversation
                                INNER JOIN useres u ON p.id_useres = u.id
                       WHERE (u.nickname = other)
                   )
               )
        OR (u.nickname = other AND c.id IN
                                    (
                                        SELECT c.id
                                        FROM conversations c
                                                 INNER JOIN participants p ON c.id = p.id_conversation
                                                 INNER JOIN useres u ON p.id_useres = u.id
                                        WHERE (u.nickname = user)
                                    )));
    RETURN hasUnread;

END $$


DELIMITER ;