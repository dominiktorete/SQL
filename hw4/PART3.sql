create table if not exists Genre(
	id SERIAL primary key,
	name VARCHAR(60)
);

create table if not exists Album(
	id SERIAL primary key,
	title VARCHAR(60),
	year INT
);

create table if not exists Artist(
	id SERIAL primary key,
	name VARCHAR(60)
);

create table if not exists Track(
	id SERIAL PRIMARY KEY,
	title VARCHAR(60),
	duration time,
	album_id INT NOT NULL references Album(id)
);

create table if not exists Comp(
	id SERIAL primary key,
	name VARCHAR(60),
	year INT
);

create table if not exists Genre_Artist(
	genre_id Integer not null references Genre(id),
	artist_id Integer not null references Artist(id),
	PRIMARY KEY (genre_id, artist_id)
);

create table if not exists Album_Artist(
	artist_id Integer not null references Artist(id),
	album_id Integer not null references Album(id),
	PRIMARY KEY (album_id, artist_id)
);


create table if not exists Comp_Track(
	comp_id Int not null references Comp(id),
	track_id Int not null references Track(id),
	PRIMARY KEY (comp_id, track_id)
);