let express = require('express');
let app = express();
let ejs = require('ejs');
const haikus = require('./haikus.json');
const port = process.env.PORT || 3000;

app.use(express.static('public'));
app.use(express.json());
app.set('view engine', 'ejs');

// Store DAW projects in memory (in production, use a database)
let projects = {};

// Original haikus route
app.get('/haikus', (req, res) => {
  res.render('index', {haikus: haikus});
});

// DAW Training App - Main route
app.get('/', (req, res) => {
  res.render('daw');
});

// API Routes for DAW functionality
app.post('/api/projects', (req, res) => {
  const projectId = Date.now().toString();
  projects[projectId] = {
    id: projectId,
    name: req.body.name || 'Untitled Project',
    tracks: [],
    tempo: 120,
    created: new Date()
  };
  res.json(projects[projectId]);
});

app.get('/api/projects/:id', (req, res) => {
  const project = projects[req.params.id];
  if (!project) {
    return res.status(404).json({ error: 'Project not found' });
  }
  res.json(project);
});

app.put('/api/projects/:id', (req, res) => {
  if (!projects[req.params.id]) {
    return res.status(404).json({ error: 'Project not found' });
  }
  projects[req.params.id] = { ...projects[req.params.id], ...req.body };
  res.json(projects[req.params.id]);
});

app.delete('/api/projects/:id', (req, res) => {
  if (!projects[req.params.id]) {
    return res.status(404).json({ error: 'Project not found' });
  }
  delete projects[req.params.id];
  res.json({ message: 'Project deleted' });
});

app.get('/api/projects', (req, res) => {
  res.json(Object.values(projects));
});

app.listen(port, () => {
  console.log(`DAW Training App running on port ${port}`);
});