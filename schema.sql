CREATE TABLE Channels (
    tunerid TEXT,
    channel INTEGER,
    program INTEGER,
    channelname TEXT
);

CREATE TABLE Schedules (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT,
    tuner TEXT,
    channel INTEGER,
    program INTEGER,
    day TEXT,
    start TEXT,
    end TEXT,
    last TEXT
);
