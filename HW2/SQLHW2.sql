create table if not exists worker (
	id SERIAL PRIMARY KEY,
	name VARCHAR(60) not null,
	department VARCHAR(30) not null,
	chief_id INT,
	foreign key (chief_id) references worker(id)
);
 