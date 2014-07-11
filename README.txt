This module allows to modify the views username autocomplete filter to only
search for usernames that are authors of nodes of selected node types.

Usage:

- add a username autocomplete filter to a view
- on the configuration form of the filter check "Filter by node type"
- select the required node type from the "Select node types" checkboxes

How this works:

- the module checks if you have selected the "Filter by node type" checkbox and
  at least one node type
- replaces the standard ("admin/views/ajax/autocomplete/user") autocomplete
  callback path for a custom one
- the machine names of the selected node types are passed in the url
- the database query is modified to only select usernames that are the authors
  of at least one node from the selected node types