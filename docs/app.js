// Library Management System - Demo JavaScript
// Uses localStorage for data persistence (no backend required)

// Initialize data structures
let resources = [];
let members = [];
let currentFilter = 'all';

// Initialize the application
document.addEventListener('DOMContentLoaded', function() {
    // Load data from localStorage
    loadFromStorage();
    
    // Load initial data
    loadStats();
    loadResources();
    loadMembers();

    // Set up form handlers
    setupForms();
    
    // Set up filter buttons
    setupFilters();
    
    // Set up resource type change handler
    setupResourceTypeChange();

    // Set up clear data button
    document.getElementById('clearData').addEventListener('click', clearAllData);
});

// Load data from localStorage
function loadFromStorage() {
    const storedResources = localStorage.getItem('libraryResources');
    const storedMembers = localStorage.getItem('libraryMembers');
    
    if (storedResources) {
        resources = JSON.parse(storedResources);
    }
    if (storedMembers) {
        members = JSON.parse(storedMembers);
    }
}

// Save data to localStorage
function saveToStorage() {
    localStorage.setItem('libraryResources', JSON.stringify(resources));
    localStorage.setItem('libraryMembers', JSON.stringify(members));
}

// Clear all data
function clearAllData() {
    if (confirm('Are you sure you want to clear all data? This cannot be undone.')) {
        resources = [];
        members = [];
        saveToStorage();
        loadStats();
        loadResources();
        loadMembers();
        showNotification('All data cleared successfully!');
    }
}

// Show notification message
function showNotification(message, isError = false) {
    const notification = document.getElementById('notification');
    notification.textContent = message;
    notification.className = 'notification ' + (isError ? 'error' : '') + ' show';
    
    setTimeout(() => {
        notification.className = 'notification';
    }, 3000);
}

// Load statistics
function loadStats() {
    document.getElementById('totalResources').textContent = resources.length;
    document.getElementById('totalMembers').textContent = members.length;
}

// Load all resources
function loadResources() {
    displayResources(resources);
}

// Display resources in the grid
function displayResources(resourcesToDisplay) {
    const container = document.getElementById('resourcesList');
    
    // Filter resources
    let filtered = resourcesToDisplay;
    if (currentFilter !== 'all') {
        filtered = resourcesToDisplay.filter(r => r.type === currentFilter);
    }
    
    if (filtered.length === 0) {
        container.innerHTML = '<p class="no-data">No resources found.</p>';
        return;
    }
    
    container.innerHTML = filtered.map(resource => {
        const unavailableClass = resource.available ? '' : 'unavailable';
        const statusClass = resource.available ? 'available' : 'unavailable';
        const statusText = resource.available ? 'Available' : 'Borrowed';
        
        let extraInfo = '';
        if (resource.type === 'Book') {
            extraInfo = `<p><strong>ISBN:</strong> ${escapeHtml(resource.isbn)}</p>
                        <p><strong>Pages:</strong> ${resource.pages}</p>`;
        } else if (resource.type === 'Magazine') {
            extraInfo = `<p><strong>Issue:</strong> ${resource.issue}</p>`;
        } else if (resource.type === 'DVD') {
            extraInfo = `<p><strong>Duration:</strong> ${resource.duration} min</p>`;
        }
        
        return `
            <div class="resource-item ${unavailableClass}">
                <span class="type-badge">${resource.type}</span>
                <h3>${escapeHtml(resource.title)}</h3>
                <p><strong>Author:</strong> ${escapeHtml(resource.author)}</p>
                ${extraInfo}
                <p><strong>ID:</strong> ${resource.id}</p>
                <span class="status ${statusClass}">${statusText}</span>
            </div>
        `;
    }).join('');
}

// Load all members
function loadMembers() {
    displayMembers(members);
}

// Display members in the grid
function displayMembers(membersToDisplay) {
    const container = document.getElementById('membersList');
    
    if (membersToDisplay.length === 0) {
        container.innerHTML = '<p class="no-data">No members registered.</p>';
        return;
    }
    
    container.innerHTML = membersToDisplay.map(member => `
        <div class="member-item">
            <h3>${escapeHtml(member.name)}</h3>
            <p><strong>Member ID:</strong> ${member.id}</p>
            <p><strong>Borrowed Items:</strong> ${member.borrowedCount}</p>
            ${member.borrowedCount > 0 ? 
                `<p><strong>Borrowed IDs:</strong> ${member.borrowedIDs.join(', ')}</p>` : 
                ''}
            <span class="borrowed-count">${member.borrowedCount}/5 borrowed</span>
        </div>
    `).join('');
}

// Set up all form handlers
function setupForms() {
    // Add Resource Form
    document.getElementById('addResourceForm').addEventListener('submit', function(e) {
        e.preventDefault();
        
        const type = document.getElementById('resourceType').value;
        const title = document.getElementById('resourceTitle').value;
        const author = document.getElementById('resourceAuthor').value;
        const extra1 = document.getElementById('resourceExtra1').value;
        const extra2 = document.getElementById('resourceExtra2').value;
        
        // Create resource object
        const newResource = {
            id: resources.length + 1,
            type: type,
            title: title,
            author: author,
            available: true,
            borrowedBy: null,
            borrowDate: null
        };
        
        // Add type-specific fields
        if (type === 'Book') {
            newResource.isbn = extra1;
            newResource.pages = parseInt(extra2);
        } else if (type === 'Magazine') {
            newResource.issue = extra1;
        } else if (type === 'DVD') {
            newResource.duration = parseInt(extra1);
        }
        
        resources.push(newResource);
        saveToStorage();
        
        showNotification(`Resource added successfully with ID: ${newResource.id}`);
        this.reset();
        updateExtraFields();
        loadStats();
        loadResources();
    });
    
    // Add Member Form
    document.getElementById('addMemberForm').addEventListener('submit', function(e) {
        e.preventDefault();
        
        const name = document.getElementById('memberName').value;
        
        const newMember = {
            id: members.length + 1,
            name: name,
            borrowedCount: 0,
            borrowedIDs: []
        };
        
        members.push(newMember);
        saveToStorage();
        
        showNotification(`Member registered successfully with ID: ${newMember.id}`);
        this.reset();
        loadStats();
        loadMembers();
    });
    
    // Borrow Form
    document.getElementById('borrowForm').addEventListener('submit', function(e) {
        e.preventDefault();
        
        const memberID = parseInt(document.getElementById('borrowMemberID').value);
        const resourceID = parseInt(document.getElementById('borrowResourceID').value);
        
        // Find member
        const member = members.find(m => m.id === memberID);
        if (!member) {
            showNotification('Member not found!', true);
            return;
        }
        
        // Check if member has reached limit
        if (member.borrowedCount >= 5) {
            showNotification('Member has reached the maximum borrowing limit (5 items)!', true);
            return;
        }
        
        // Find resource
        const resource = resources.find(r => r.id === resourceID);
        if (!resource) {
            showNotification('Resource not found!', true);
            return;
        }
        
        // Check if resource is available
        if (!resource.available) {
            showNotification('Resource is not available!', true);
            return;
        }
        
        // Borrow the resource
        resource.available = false;
        resource.borrowedBy = memberID;
        resource.borrowDate = new Date().toISOString();
        
        member.borrowedCount++;
        member.borrowedIDs.push(resourceID);
        
        saveToStorage();
        
        showNotification('Resource borrowed successfully!');
        this.reset();
        loadStats();
        loadResources();
        loadMembers();
    });
    
    // Return Form
    document.getElementById('returnForm').addEventListener('submit', function(e) {
        e.preventDefault();
        
        const memberID = parseInt(document.getElementById('returnMemberID').value);
        const resourceID = parseInt(document.getElementById('returnResourceID').value);
        
        // Find member
        const member = members.find(m => m.id === memberID);
        if (!member) {
            showNotification('Member not found!', true);
            return;
        }
        
        // Find resource
        const resource = resources.find(r => r.id === resourceID);
        if (!resource) {
            showNotification('Resource not found!', true);
            return;
        }
        
        // Check if resource is borrowed
        if (resource.available) {
            showNotification('Resource is not borrowed!', true);
            return;
        }
        
        // Check if this member borrowed this resource
        if (resource.borrowedBy !== memberID) {
            showNotification('This member did not borrow this resource!', true);
            return;
        }
        
        // Calculate late fee (simplified - just for demo)
        const borrowDate = new Date(resource.borrowDate);
        const returnDate = new Date();
        const daysDiff = Math.floor((returnDate - borrowDate) / (1000 * 60 * 60 * 24));
        
        let lateFee = 0;
        if (daysDiff > 14) { // Assuming 14 days borrowing period
            const lateDays = daysDiff - 14;
            if (resource.type === 'Book') {
                lateFee = lateDays * 5;
            } else if (resource.type === 'Magazine') {
                lateFee = lateDays * 3;
            } else if (resource.type === 'DVD') {
                lateFee = lateDays * 10;
            }
        }
        
        // Return the resource
        resource.available = true;
        resource.borrowedBy = null;
        resource.borrowDate = null;
        
        member.borrowedCount--;
        member.borrowedIDs = member.borrowedIDs.filter(id => id !== resourceID);
        
        saveToStorage();
        
        let message = 'Resource returned successfully!';
        if (lateFee > 0) {
            message += ` Late fee: Rs. ${lateFee}`;
        }
        
        showNotification(message);
        this.reset();
        loadStats();
        loadResources();
        loadMembers();
    });
}

// Set up filter buttons
function setupFilters() {
    const filterBtns = document.querySelectorAll('.filter-btn');
    
    filterBtns.forEach(btn => {
        btn.addEventListener('click', function() {
            // Update active button
            filterBtns.forEach(b => b.classList.remove('active'));
            this.classList.add('active');
            
            // Update filter and reload resources
            currentFilter = this.dataset.filter;
            loadResources();
        });
    });
}

// Set up resource type change handler
function setupResourceTypeChange() {
    const typeSelect = document.getElementById('resourceType');
    typeSelect.addEventListener('change', updateExtraFields);
}

// Update extra fields based on resource type
function updateExtraFields() {
    const type = document.getElementById('resourceType').value;
    const extra1Label = document.getElementById('extra1Label');
    const extra2Label = document.getElementById('extra2Label');
    const extra1Input = document.getElementById('resourceExtra1');
    const extra2Input = document.getElementById('resourceExtra2');
    
    if (type === 'Book') {
        extra1Label.textContent = 'ISBN:';
        extra2Label.textContent = 'Pages:';
        extra1Input.type = 'text';
        extra2Input.type = 'number';
        extra2Input.required = true;
    } else if (type === 'Magazine') {
        extra1Label.textContent = 'Issue Number:';
        extra2Label.textContent = 'Issue Number:';
        extra1Input.type = 'number';
        extra2Input.type = 'hidden';
        extra2Input.required = false;
    } else if (type === 'DVD') {
        extra1Label.textContent = 'Duration (minutes):';
        extra2Label.textContent = 'Duration (minutes):';
        extra1Input.type = 'number';
        extra2Input.type = 'hidden';
        extra2Input.required = false;
    }
}

// Escape HTML to prevent XSS
function escapeHtml(text) {
    const div = document.createElement('div');
    div.textContent = text;
    return div.innerHTML;
}