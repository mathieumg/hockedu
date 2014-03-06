CREATE TABLE [dbo].[users]
(
    [Id] INT NOT NULL PRIMARY KEY IDENTITY, 
    [username] TEXT NOT NULL, 
    [password] TEXT NOT NULL, 
    [password_salt] TEXT NOT NULL, 
    [email] TEXT NOT NULL, 
    [registration_time] TIME NOT NULL
)
