// Library Management System - Frontend JavaScript
// Connects to C++ Backend API

const API_BASE = 'http://localhost:8080';

// Current filter for resources
let currentFilter = 'all';

// Initialize the application
document.addEventListener('DOMContentLoaded', function() {
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
});

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
async function loadStats() {
    try {
        const response = await fetch(`${API_BASE}/api/stats`);
        const data = await response.json();
        
        document.getElementById('totalResources').textContent = data.resources;
        document.getElementById('totalMembers').textContent = data.members;
    } catch (error) {
        console.error('Error loading stats:', error);
    }
}

// Load all resources
async function loadResources() {
    try {
        const response = await fetch(`${API_BASE}/api/resources`);
        const resources = await response.json();
        
        displayResources(resources);
    } catch (error) {
        console.error('Error loading resources:', error);
        showNotification('Error loading resources', true);
    }
}

// Display resources in the grid
function displayResources(resources) {
    const container = document.getElementById('resourcesList');
    
    // Filter resources
    let filtered = resources;
    if (currentFilter !== 'all') {
        filtered = resources.filter(r => r.type === currentFilter);
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
            extraInfo = `<p><strong>ISBN:</strong> ${resource.isbn}</p>
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
async function loadMembers() {
    try {
        const response = await fetch(`${API_BASE}/api/members`);
        const members = await response.json();
        
        displayMembers(members);
    } catch (error) {
        console.error('Error loading members:', error);
        showNotification('Error loading members', true);
    }
}

// Display members in the grid
function displayMembers(members) {
    const container = document.getElementById('membersList');
    
    if (members.length === 0) {
        container.innerHTML = '<p class="no-data">No members registered.</p>';
        return;
    }
    
    container.innerHTML = members.map(member => `
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
    document.getElementById('addResourceForm').addEventListener('submit', async function(e) {
        e.preventDefault();
        
        const type = document.getElementById('resourceType').value;
        const title = document.getElementById('resourceTitle').value;
        const author = document.getElementById('resourceAuthor').value;
        const extra1 = document.getElementById('resourceExtra1').value;
        const extra2 = document.getElementById('resourceExtra2').value;
        
        const formData = new FormData();
        formData.append('type', type);
        formData.append('title', title);
        formData.append('author', author);
        formData.append('extra1', extra1);
        formData.append('extra2', extra2);
        
        try {
            const response = await fetch(`${API_BASE}/api/resources`, {
                method: 'POST',
                body: formData
            });
            
            const result = await response.json();
            
            if (result.success) {
                showNotification(`Resource added successfully with ID: ${result.id}`);
                this.reset();
                // Reset extra fields based on type
                updateExtraFields();
                loadStats();
                loadResources();
            } else {
                showNotification('Error adding resource', true);
            }
        } catch (error) {
            console.error('Error:', error);
            showNotification('Error adding resource', true);
        }
    });
    
    // Add Member Form
    document.getElementById('addMemberForm').addEventListener('submit', async function(e) {
        e.preventDefault();
        
        const name = document.getElementById('memberName').value;
        
        const formData = new FormData();
        formData.append('name', name);
        
        try {
            const response = await fetch(`${API_BASE}/api/members`, {
                method: 'POST',
                body: formData
            });
            
            const result = await response.json();
            
            if (result.success) {
                showNotification(`Member registered successfully with ID: ${result.id}`);
                this.reset();
                loadStats();
                loadMembers();
            } else {
                showNotification('Error registering member', true);
            }
        } catch (error) {
            console.error('Error:', error);
            showNotification('Error registering member', true);
        }
    });
    
    // Borrow Form
    document.getElementById('borrowForm').addEventListener('submit', async function(e) {
        e.preventDefault();
        
        const memberID = document.getElementById('borrowMemberID').value;
        const resourceID = document.getElementById('borrowResourceID').value;
        
        const formData = new FormData();
        formData.append('memberID', memberID);
        formData.append('resourceID', resourceID);
        
        try {
            const response = await fetch(`${API_BASE}/api/borrow`, {
                method: 'POST',
                body: formData
            });
            
            const result = await response.json();
            
            if (result.success) {
                showNotification('Resource borrowed successfully!');
                this.reset();
                loadStats();
                loadResources();
                loadMembers();
            } else {
                showNotification('Error borrowing resource. Check if resource is available or member has reached limit.', true);
            }
        } catch (error) {
            console.error('Error:', error);
            showNotification('Error borrowing resource', true);
        }
    });
    
    // Return Form
    document.getElementById('returnForm').addEventListener('submit', async function(e) {
        e.preventDefault();
        
        const memberID = document.getElementById('returnMemberID').value;
        const resourceID = document.getElementById('returnResourceID').value;
        
        const formData = new FormData();
        formData.append('memberID', memberID);
        formData.append('resourceID', resourceID);
        
        try {
            const response = await fetch(`${API_BASE}/api/return`, {
                method: 'POST',
                body: formData
            });
            
            const result = await response.json();
            
            if (result.success) {
                showNotification('Resource returned successfully!');
                this.reset();
                loadStats();
                loadResources();
                loadMembers();
            } else {
                showNotification('Error returning resource. Check member ID and resource ID.', true);
            }
        } catch (error) {
            console.error('Error:', error);
            showNotification('Error returning resource', true);
        }
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