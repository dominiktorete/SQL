--hw4-2
select title, duration
from track;

select title 
from track
where duration >= '00:03:30';

select name
from comp 
where year between 2018 and 2020;

select name
from artist
where name not like '% %';

select title 
from track
where title like '%my%' or title like '%мой%';

--hw4-3

select name, COUNT(artist_id)
from genre
left join genre_artist on genre.id  = genre_artist.genre_id 
group by name;

select album.title, year, COUNT(track.id) count_track
from album
left join track on album.id = track.album_id
where year between 2019 and 2020
group by album.title, year;

select album.title, AVG(track.duration) duration_tracks
from album
left join track on album.id = track.album_id
group by album.title;

select name
from artist
left join album_artist on artist.id = album_artist.artist_id 
left join album on album_artist.album_id = album.id 
where album.year != 2020
group by name;

select comp.name
from comp
left join comp_track ct on comp.id  = ct.comp_id 
left join track tr on ct.track_id = tr.id
left join album al on tr.album_id = al.id 
left join album_artist aa on al.id = aa.album_id 
left join artist a on aa.artist_id = a.id
where a.name = 'John Smith'
group by comp.name;

--hw4-4

select album.title, COUNT(genre_id) count_genre
from album
left join album_artist aa on album.id  = aa.album_id 
left join artist a on aa.artist_id = a.id 
left join genre_artist ga on a.id = ga.artist_id 
left join genre g on ga.genre_id = g.id 
group by album.title 
having COUNT(genre_id) > 1;

select track.title
from track
left join comp_track ct on track.id  = ct.track_id
where ct.comp_id is NULL
group by track.title;

select artist.name
from artist
left join album_artist aa on artist.id = aa.artist_id 
left join album a on aa.album_id = a.id 
left join track t on a.id = t.album_id 
where t.duration = (select MIN(duration) from track)
group by artist.name

select album.title, COUNT(album.id) count_track
from album
left join track t on album.id = t.album_id 
group by album.title
having COUNT(album.id) = (select MIN(cnt) from (select COUNT(album.id) as cnt
							from album
							left join track t on album.id = t.album_id
							group by album.id) as count
						);
