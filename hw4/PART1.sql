INSERT INTO artist(id, name) 
VALUES(1,'John Smith'), (2,'Max'), (3,'Egor Serv'), (4,'Kir');

INSERT INTO genre(id, name) 
VALUES(1,'Rock'), (2,'Hip-hop'), (3,'Rap');

INSERT INTO album(id, title, year) 
VALUES(1,'ALBUM1', 1992), (2,'ALBUM2', 2000), (3,'ALBUM3', 2023);

INSERT INTO track(id, title, duration, album_id) 
values (1,'Into you', '00:03:30', 1), (2,'Get low', '00:03:00', 1), (3,'Ты мой', '00:02:10', 2),
(4,'family my', '00:03:40', 2), (5,'Моя весна', '00:02:10', 3), (6,'Антигейша', '00:02:50', 3),(7,'Весна', '00:02:35', 3) ,(8,'Лето', '00:03:35', 3) ;

INSERT INTO comp(id, name, year) 
values (1,'COMP1', 2000), (2,'COMP2', 2003),
(3,'COMP3', 1998), (4,'COMP4', 2005);

INSERT INTO genre_artist(genre_id, artist_id) 
values (2,1), (1, 2), (1, 3), (3, 4);

INSERT INTO album_artist (album_id, artist_id) 
values (1, 1), (1, 2), (2, 3), (3, 4);

INSERT INTO comp_track (comp_id, track_id) 
values (1, 1), (2, 2), (2, 3), (3, 4), (4, 5), (4, 6);