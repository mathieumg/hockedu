CREATE TABLE [dbo].[comm_log]
(
    [Id] INT NOT NULL PRIMARY KEY IDENTITY, 
    [user_id] INT NOT NULL, 
    [type] INT NOT NULL, 
    [data] TEXT NOT NULL, 
    [ipadress] TEXT NOT NULL, 
    [time] TIME NOT NULL
)
