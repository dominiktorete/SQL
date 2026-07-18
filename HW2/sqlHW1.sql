create table if not exists SONGER (
	id SERIAL PRIMARY KEY,
	name VARCHAR(60) not null
);

create table if not exists JANRE (
	id SERIAL PRIMARY KEY,
	name VARCHAR(60) not null
);

create table if not exists ALBUM (
	id SERIAL PRIMARY KEY,
	name VARCHAR(60) not null,
	year INT not null
);

create table if not exists TRACK (
	id INTEGER PRIMARY KEY references ALBUM(id), 
	name VARCHAR(60) not null,
	duration REAL not null
);

create table if not exists SBORNIK (
	id SERIAL PRIMARY KEY,
	name VARCHAR(60) not null,
	year INT not null
);

create table if not exists ALBUM_SONGER (
	songer_id INTEGER references SONGER(id),
	janre_id INTEGER references JANRE(id)
);

create table if not exists ALBUM_SONGER (
	songer_id INTEGER references SONGER(id),
	album_id INTEGER references ALBUM(id)
);

create table if not exists SBORNIK_TRACKS (
	sbornick_id INTEGER references SBORNIK(id),
	track_id INTEGER references TRACK(id)
);